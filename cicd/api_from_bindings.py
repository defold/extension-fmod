import re
from pathlib import Path
from typing import Any, Protocol

from jinja2 import Environment, FileSystemLoader

from util import log

_SCRIPT_DIR = Path(__file__).resolve().parent
_FMOD_HEADER = _SCRIPT_DIR.parent / "fmod" / "include" / "fmod_common.h"


def _parse_fmod_doc_version() -> str:
    text = _FMOD_HEADER.read_text()
    m = re.search(r"#define\s+FMOD_VERSION\s+0x([0-9a-fA-F]{8})", text)
    if not m:
        return "2.03"
    hex_val = int(m.group(1), 16)
    product = (hex_val >> 16) & 0xFFFF
    major = (hex_val >> 8) & 0xFF
    return f"{product}.{major:02d}"


FMOD_DOC_VERSION = _parse_fmod_doc_version()


class ParsedStruct(Protocol):
    name: str
    is_class: bool
    methods: list[tuple[str, Any]]
    properties: list[Any]


class ParsedTypeDecl(Protocol):
    c_type: str
    type: int


class ParsedMethod(Protocol):
    name: str
    args: list[Any]
    generated: bool


TYPE_BASIC = 1
TYPE_STRUCT = 2
TYPE_CLASS = 3
TYPE_POINTER = 4

_SNAKE_CASE_RE = re.compile(r"^_*(IDs|[A-Z][a-z]+|[A-Z0-9]+(?![a-z]))")
_CAMEL_SPLIT_RE = re.compile(r"([A-Z][a-z]+|[A-Z]+(?=[A-Z][a-z]|\d|\b)|\d+)")

INPUT_USAGES = ("input", "input_ptr", "input_deref")
OUTPUT_USAGES = ("output", "output_ptr")

BASIC_NUMERIC_TYPES = {"float", "double"}
INTEGER_TYPES = {"int", "short", "long", "char", "FMOD_BOOL"}

TYPE_HANDLERS = {
    TYPE_BASIC: lambda c_type: _convert_basic_type(c_type),
    TYPE_POINTER: lambda c_type: _convert_pointer_type(c_type),
    TYPE_STRUCT: lambda c_type: _convert_struct_type(c_type),
    TYPE_CLASS: lambda c_type: _convert_struct_type(c_type),
}

PARAM_DESCRIPTIONS = {
    "system": "FMOD system handle",
    "sound": "Sound handle",
    "channel": "Channel handle",
    "channelgroup": "Channel group handle",
    "soundgroup": "Sound group handle",
    "dsp": "DSP handle",
    "dspconnection": "DSP connection handle",
    "geometry": "Geometry handle",
    "reverb": "Reverb handle",
    "reverb3d": "3D reverb handle",
    "bus": "Bus handle",
    "vca": "VCA handle",
    "bank": "Bank handle",
    "eventdescription": "Event description handle",
    "eventinstance": "Event instance handle",
    "name": "Name or path",
    "filename": "File path",
    "path": "Path string",
    "length": "Length value",
    "size": "Size in bytes",
    "mode": "Mode flags",
    "volume": "Volume level (0.0 to 1.0)",
    "position": "Position value",
    "paused": "Paused state",
    "index": "Index value",
    "frequency": "Frequency in Hz",
    "pitch": "Pitch value (0.5 to 2.0)",
    "pan": "Pan position (-1.0 left to 1.0 right)",
    "priority": "Priority (0 = highest, 256 = lowest)",
    "samplerate": "Sample rate in Hz",
    "delay": "Delay value",
    "rate": "Rate value",
    "count": "Number of items",
    "numchannels": "Number of audio channels",
    "speakermode": "Speaker mode value",
    "flags": "Configuration flags",
    "guid": "FMOD GUID",
    "attributes": "3D attributes",
    "mask": "Channel mask",
    "label": "Label string",
    "id": "Identifier",
    "gain": "Gain level in dB",
    "wet": "Wet mix level",
    "dry": "Dry mix level",
    "mix": "Mix level",
    "angle": "Angle in degrees",
    "level": "Level value",
    "mindistance": "Minimum distance for 3D attenuation",
    "maxdistance": "Maximum distance for 3D attenuation",
    "loop": "Loop count (-1 = infinite)",
    "loopcount": "Loop count (-1 = infinite)",
    "velocity": "3D velocity vector",
    "orientation": "3D orientation",
    "forward": "Forward orientation vector",
    "up": "Up orientation vector",
    "numbuffers": "Number of buffers",
    "bufferlength": "Buffer length in samples",
    "driver": "Driver index",
    "output": "Output type",
    "speakermix": "Speaker mix levels",
    "reverbproperties": "Reverb properties",
    "description": "Description string",
    "timeout": "Timeout in milliseconds",
    "type": "Type value",
    "value": "Parameter value",
    "finalvalue": "Final computed parameter value",
    "ignoreseekspeed": "Whether to ignore seek speed",
}

CLASS_DOC_URLS = {
    "FMOD_SYSTEM": "core-api-system",
    "FMOD_SOUND": "core-api-sound",
    "FMOD_CHANNEL": "core-api-channel",
    "FMOD_CHANNELCONTROL": "core-api-channelcontrol",
    "FMOD_CHANNELGROUP": "core-api-channelgroup",
    "FMOD_SOUNDGROUP": "core-api-soundgroup",
    "FMOD_DSP": "core-api-dsp",
    "FMOD_DSPCONNECTION": "core-api-dspconnection",
    "FMOD_GEOMETRY": "core-api-geometry",
    "FMOD_REVERB3D": "core-api-reverb3d",
    "FMOD_STUDIO_SYSTEM": "studio-api-system",
    "FMOD_STUDIO_EVENTDESCRIPTION": "studio-api-eventdescription",
    "FMOD_STUDIO_EVENTINSTANCE": "studio-api-eventinstance",
    "FMOD_STUDIO_BUS": "studio-api-bus",
    "FMOD_STUDIO_VCA": "studio-api-vca",
    "FMOD_STUDIO_BANK": "studio-api-bank",
    "FMOD_STUDIO_COMMANDREPLAY": "studio-api-commandreplay",
}

CLASS_DESCRIPTIONS = {
    "FMOD_SYSTEM": "The core FMOD system object. Handles device enumeration, audio output, channel management, and DSP processing.",
    "FMOD_SOUND": "A loaded audio sample or stream. Represents audio data that can be played on channels.",
    "FMOD_CHANNEL": "A playing instance of a sound. Owned by the system and cannot be released manually.",
    "FMOD_CHANNELCONTROL": "Base interface shared by channels and channel groups for volume, pitch, and 3D control.",
    "FMOD_CHANNELGROUP": "A group of channels. Used for submixing, organizing audio, and applying shared effects.",
    "FMOD_SOUNDGROUP": "A group of sounds. Controls limits on simultaneous playback and shared behavior.",
    "FMOD_DSP": "A DSP effect unit. Can be connected into the signal chain to process audio.",
    "FMOD_DSPCONNECTION": "A connection between two DSP units in the signal graph.",
    "FMOD_GEOMETRY": "A geometry object for occlusion-based 3D audio.",
    "FMOD_REVERB3D": "A 3D reverb zone that applies reverb to sounds within its sphere of influence.",
    "FMOD_POLYGON": "A polygon belonging to a geometry object.",
    "FMOD_SYNCPOINT": "A sync point marker within a sound.",
    "FMOD_STUDIO_SYSTEM": "The FMOD Studio system object. Manages banks, events, buses, and the Studio runtime.",
    "FMOD_STUDIO_EVENTDESCRIPTION": "Describes a Studio event. Used to query event properties and create playback instances.",
    "FMOD_STUDIO_EVENTINSTANCE": "A playing instance of a Studio event. Controls playback, parameters, and 3D attributes. Automatically garbage collected.",
    "FMOD_STUDIO_BUS": "A mixer bus from the Studio project. Controls volume, pausing, and effects for routed signals.",
    "FMOD_STUDIO_VCA": "A VCA fader from the Studio project. Controls volume for assigned buses.",
    "FMOD_STUDIO_BANK": "A loaded FMOD Studio bank containing events, samples, and metadata.",
    "FMOD_STUDIO_COMMANDREPLAY": "Replays a previously recorded sequence of Studio API commands.",
}

CLASSES_REQUIRING_RELEASE = {
    "FMOD_SOUND", "FMOD_CHANNELGROUP", "FMOD_SOUNDGROUP", "FMOD_DSP",
    "FMOD_GEOMETRY", "FMOD_REVERB3D",
    "FMOD_STUDIO_BANK", "FMOD_STUDIO_COMMANDREPLAY",
}

_ENUM_SEP = " - "

ENUM_GROUPS = {
    "OK": "Operation completed successfully.",
    "ERR_": f"Error{_ENUM_SEP}{{}}.",
    "OUTPUTTYPE_": f"Output type{_ENUM_SEP}{{}}.",
    "SPEAKERMODE_": f"Speaker mode{_ENUM_SEP}{{}}.",
    "SOUND_TYPE_": f"Sound file type{_ENUM_SEP}{{}}.",
    "SOUND_FORMAT_": f"Sound data format{_ENUM_SEP}{{}}.",
    "DSP_TYPE_": f"DSP effect type{_ENUM_SEP}{{}}.",
    "DSP_RESAMPLER_": f"DSP resampler{_ENUM_SEP}{{}}.",
    "DSP_CALLBACK_": f"DSP callback type{_ENUM_SEP}{{}}.",
    "DSP_PAN_SURROUND_": f"Pan surround mode{_ENUM_SEP}{{}}.",
    "DSP_PAN_3D_ROLLOFF_": f"Pan 3D rolloff type{_ENUM_SEP}{{}}.",
    "CHANNELCONTROL_": f"Channel control type{_ENUM_SEP}{{}}.",
    "CHANNELCONTROL_DSP_INDEX_": f"Channel control DSP index{_ENUM_SEP}{{}}.",
    "CHANNELCONTROL_CALLBACK_": f"Channel control callback{_ENUM_SEP}{{}}.",
    "CHANNELORDER_": f"Channel order{_ENUM_SEP}{{}}.",
    "PLUGINTYPE_": f"Plugin type{_ENUM_SEP}{{}}.",
    "OPENSTATE_": f"Open state{_ENUM_SEP}{{}}.",
    "SOUNDGROUP_BEHAVIOR_": f"Sound group behavior{_ENUM_SEP}{{}}.",
    "TAGTYPE_": f"Tag type{_ENUM_SEP}{{}}.",
    "TAGDATATYPE_": f"Tag data type{_ENUM_SEP}{{}}.",
    "DRIVER_STATE_": f"Driver state{_ENUM_SEP}{{}}.",
    "TIMEUNIT_": f"Time unit{_ENUM_SEP}{{}}.",
    "THREAD_PRIORITY_": f"Thread priority{_ENUM_SEP}{{}}.",
    "THREAD_STACK_SIZE_": f"Thread stack size{_ENUM_SEP}{{}}.",
    "THREAD_AFFINITY_": f"Thread affinity{_ENUM_SEP}{{}}.",
    "THREAD_TYPE_": f"Thread type{_ENUM_SEP}{{}}.",
    "RESULT_": f"Result{_ENUM_SEP}{{}}.",
    "DEBUG_MODE_": f"Debug mode{_ENUM_SEP}{{}}.",
    "STUDIO_PLAYBACK_": f"Playback state{_ENUM_SEP}{{}}.",
    "STUDIO_STOP_": f"Stop mode{_ENUM_SEP}{{}}.",
    "STUDIO_LOADING_STATE_": f"Loading state{_ENUM_SEP}{{}}.",
    "STUDIO_LOAD_BANK_": f"Bank loading mode{_ENUM_SEP}{{}}.",
    "STUDIO_PARAMETER_TYPE_": f"Parameter type{_ENUM_SEP}{{}}.",
    "STUDIO_USER_PROPERTY_TYPE_": f"User property type{_ENUM_SEP}{{}}.",
    "STUDIO_EVENT_PROPERTY_": f"Event property{_ENUM_SEP}{{}}.",
    "STUDIO_INSTANCETYPE_": f"Instance type{_ENUM_SEP}{{}}.",
    "PORT_TYPE_": f"Port type{_ENUM_SEP}{{}}.",
    "STUDIO_EVENT_CALLBACK_": f"Event callback type{_ENUM_SEP}{{}}.",
    "STUDIO_SYSTEM_CALLBACK_": f"System callback type{_ENUM_SEP}{{}}.",
    "SPEAKER_": f"Speaker position{_ENUM_SEP}{{}}.",
    "DSPCONNECTION_TYPE_": f"DSP connection type{_ENUM_SEP}{{}}.",
    "DSP_PARAMETER_TYPE_": f"DSP parameter type{_ENUM_SEP}{{}}.",
    "STUDIO_PARAMETER_": f"Studio parameter type{_ENUM_SEP}{{}}.",
}


def _convert_basic_type(c_type: str) -> str:
    if "FMOD_VECTOR" in c_type:
        return "vector3"
    if c_type in BASIC_NUMERIC_TYPES:
        return "number"
    if any(base_type in c_type for base_type in INTEGER_TYPES):
        return "boolean" if "FMOD_BOOL" in c_type else "number"
    if c_type.startswith("FMOD_"):
        return "number"
    return "number"


def _convert_pointer_type(c_type: str) -> str:
    if "FMOD_VECTOR" in c_type:
        return "vector3"
    if "char" in c_type:
        return "string"
    return "userdata"


def _convert_struct_type(c_type: str) -> str:
    if "FMOD_VECTOR" in c_type:
        return "vector3"
    type_lower = c_type.lower()
    if type_lower.startswith("fmod_studio_"):
        return type_lower.replace("fmod_studio_", "fmod.studio.")
    if type_lower.startswith("fmod_"):
        return type_lower.replace("fmod_", "fmod.")
    return "userdata"


def convert_to_snake_case(text: str) -> str:
    components = []
    remaining = text
    while True:
        match = _SNAKE_CASE_RE.match(remaining)
        if match is None:
            break
        components.append(match.group(1).lower())
        remaining = remaining[match.end() :]
    return "_".join(components)


def convert_c_type_to_lua_type(c_type: str, type_enum: int) -> str:
    handler = TYPE_HANDLERS.get(type_enum)
    if handler:
        return handler(c_type)
    return "any"


def _class_to_readable(name: str) -> str:
    stripped = re.sub(r"^FMOD_(STUDIO_)?", "", name)
    words = _CAMEL_SPLIT_RE.findall(stripped)
    if words:
        return " ".join(w.lower() for w in words)
    chunks = stripped.split("_")
    return " ".join(c.lower() for c in chunks if c)


def _camel_to_words(text: str) -> str:
    words = _CAMEL_SPLIT_RE.findall(text)
    if words:
        return " ".join(w.lower() for w in words)
    return text.lower()


def _extract_verb_subject(c_func_name: str, class_name: str) -> str:
    func_upper = c_func_name.upper()
    prefix = class_name.upper() + "_"
    if func_upper.startswith(prefix):
        return c_func_name[len(prefix):]
    parts = c_func_name.split("_")
    parts_upper = func_upper.split("_")
    class_parts = class_name.upper().split("_")
    num_class_parts = len(class_parts)
    if parts_upper[:num_class_parts] == class_parts and len(parts) > num_class_parts:
        return "_".join(parts[num_class_parts:])
    return parts[-1] if parts else c_func_name


def generate_function_description(c_func_name: str, class_name: str) -> str:
    action_part = _extract_verb_subject(c_func_name, class_name)
    readable_class = _class_to_readable(class_name)
    doc_url = CLASS_DOC_URLS.get(class_name, "")

    patterns = [
        (r"^Set(.+)$", lambda m: f"Sets the {_camel_to_words(m.group(1))} of the {readable_class}."),
        (r"^Get(.+)$", lambda m: f"Gets the {_camel_to_words(m.group(1))} of the {readable_class}."),
        (r"^Is(.+)$", lambda m: f"Returns whether the {readable_class} is {_camel_to_words(m.group(1))}."),
        (r"^Has(.+)$", lambda m: f"Returns whether the {readable_class} has {_camel_to_words(m.group(1))}."),
        (r"^Create(.+)$", lambda m: f"Creates a new {_camel_to_words(m.group(1))}."),
        (r"^Release$", lambda m: f"Releases the {readable_class}."),
        (r"^Load(.+)$", lambda m: f"Loads a {_camel_to_words(m.group(1))}."),
        (r"^Unload(.+)$", lambda m: f"Unloads a {_camel_to_words(m.group(1))}."),
        (r"^Start$", lambda m: f"Starts the {readable_class}."),
        (r"^Stop$", lambda m: f"Stops the {readable_class}."),
        (r"^Add(.+)$", lambda m: f"Adds {_camel_to_words(m.group(1))} to the {readable_class}."),
        (r"^Remove(.+)$", lambda m: f"Removes {_camel_to_words(m.group(1))} from the {readable_class}."),
        (r"^Lock$", lambda m: f"Locks the {readable_class}."),
        (r"^Unlock$", lambda m: f"Unlocks the {readable_class}."),
        (r"^Flush(.*)$", lambda m: f"Flushes {_camel_to_words(m.group(1)) + ' from ' if m.group(1) else ''}the {readable_class}."),
        (r"^Play(.+)$", lambda m: f"Plays {_camel_to_words(m.group(1))}."),
        (r"^Update$", lambda m: f"Updates the {readable_class}."),
        (r"^Reset(.*)$", lambda m: f"Resets {_camel_to_words(m.group(1)) + ' of ' if m.group(1) else ''}the {readable_class}."),
        (r"^Initialize$", lambda m: f"Initializes the {readable_class}."),
        (r"^Init$", lambda m: f"Initializes the {readable_class}."),
        (r"^Close$", lambda m: f"Closes the {readable_class}."),
        (r"^Read(.*)$", lambda m: f"Reads {_camel_to_words(m.group(1)) if m.group(1) else 'data'} from the {readable_class}."),
        (r"^Write(.*)$", lambda m: f"Writes {_camel_to_words(m.group(1)) if m.group(1) else 'data'} to the {readable_class}."),
        (r"^Seek(.*)$", lambda m: f"Seeks to {_camel_to_words(m.group(1)) if m.group(1) else 'a position'} in the {readable_class}."),
    ]

    for pattern, formatter in patterns:
        m = re.match(pattern, action_part)
        if m:
            desc = formatter(m)
            if doc_url:
                desc += f"\n\nSee [FMOD docs](https://www.fmod.com/docs/{FMOD_DOC_VERSION}/api/{doc_url}.html)."
            return desc

    desc = _camel_to_words(action_part).capitalize() + f" on the {readable_class}."
    if doc_url:
        desc += f"\n\n    See [FMOD docs](https://www.fmod.com/docs/{FMOD_DOC_VERSION}/api/{doc_url}.html)."
    return desc


def generate_global_func_description(c_func_name: str) -> str:
    stripped = re.sub(r"^FMOD_(Studio_)?", "", c_func_name)
    parts = stripped.split("_", 1)
    if len(parts) == 2:
        subject = _camel_to_words(parts[0])
        action = _camel_to_words(parts[1])
        return f"{action.capitalize()} for {subject}."
    return _camel_to_words(stripped).capitalize() + "."


def get_class_description(class_name: str) -> str:
    desc = CLASS_DESCRIPTIONS.get(class_name, f"{_class_to_readable(class_name).capitalize()}.")
    doc_url = CLASS_DOC_URLS.get(class_name, "")
    if doc_url:
        desc += f"\n\nSee [FMOD docs](https://www.fmod.com/docs/{FMOD_DOC_VERSION}/api/{doc_url}.html)."
    return desc


def get_class_notes(class_name: str) -> str:
    if class_name in CLASSES_REQUIRING_RELEASE:
        return "Must be released manually by calling release() when no longer needed."
    if class_name == "FMOD_STUDIO_EVENTINSTANCE":
        return "Automatically garbage collected by Lua. No manual release needed."
    if class_name == "FMOD_CHANNEL":
        return "Owned by the system. Cannot be released manually."
    return ""


def generate_parameter_description(param_name: str, c_type: str = "", type_enum: int = 0) -> str:
    param_lower = param_name.lower()

    if param_lower in PARAM_DESCRIPTIONS:
        return PARAM_DESCRIPTIONS[param_lower]

    if c_type and c_type.startswith("FMOD_") and type_enum == TYPE_BASIC:
        readable = _class_to_readable(c_type)
        if readable:
            return f"{readable.capitalize()} value"

    if type_enum == TYPE_CLASS:
        readable = _class_to_readable(c_type)
        if readable:
            return f"{readable.capitalize()} handle"

    if c_type == "FMOD_VECTOR" or "FMOD_VECTOR" in c_type:
        return "3D position vector"

    for key, description in PARAM_DESCRIPTIONS.items():
        if key in param_lower:
            return description

    words = param_lower.replace("_", " ")
    if words != param_lower:
        return words.capitalize()
    return _camel_to_words(param_name).capitalize() if param_name != param_lower else param_name


def generate_return_description(arg_name: str, c_type: str, type_enum: int) -> str:
    lua_type = convert_c_type_to_lua_type(c_type, type_enum)
    readable_name = _camel_to_words(arg_name)

    if lua_type == "boolean":
        return f"True if {readable_name}, false otherwise"
    if lua_type == "string":
        return f"The {readable_name} string"
    if lua_type == "vector3":
        return f"The {readable_name} vector"
    if type_enum in (TYPE_STRUCT, TYPE_CLASS):
        readable_type = _class_to_readable(c_type)
        return f"The {readable_type}"
    return f"The {readable_name} value"


def generate_enum_description(name: str) -> str:
    if name == "OK":
        return ENUM_GROUPS["OK"]

    for prefix, template in sorted(ENUM_GROUPS.items(), key=lambda x: -len(x[0])):
        if prefix == "OK":
            continue
        if name.startswith(prefix):
            suffix = name[len(prefix):]
            value = suffix.lower().replace("_", " ").strip()
            if not value:
                value = suffix
            return template.format(value)

    parts = name.split("_")
    return " ".join(p.lower() for p in parts).capitalize() + "."


def is_sentinel_enum(name: str) -> bool:
    if name.endswith("_FORCEINT"):
        return True
    if not name.endswith("_MAX"):
        return False
    prefix = name[: -len("MAX")]
    return any(key == prefix for key in ENUM_GROUPS)


def generate_field_description(field_name: str, c_type: str, struct_name: str) -> str:
    field_lower = field_name.lower()

    field_descs = {
        "volume": "Volume level",
        "pitch": "Pitch value",
        "pan": "Pan position",
        "flags": "Configuration flags",
        "position": "Position value",
        "velocity": "Velocity vector",
        "forward": "Forward orientation vector",
        "up": "Up orientation vector",
        "frequency": "Frequency in Hz",
        "priority": "Priority value",
        "mindistance": "Minimum distance for 3D attenuation",
        "maxdistance": "Maximum distance for 3D attenuation",
        "gain": "Gain level",
        "name": "Name string",
        "size": "Size in bytes",
        "length": "Length value",
        "count": "Item count",
        "index": "Index value",
        "type": "Type identifier",
        "mode": "Mode flags",
        "format": "Data format",
        "channels": "Number of channels",
        "samplerate": "Sample rate in Hz",
        "bits": "Bit depth",
    }

    if field_lower in field_descs:
        return field_descs[field_lower]

    for key, desc in field_descs.items():
        if key in field_lower:
            return desc

    lua_type = convert_c_type_to_lua_type(c_type, TYPE_BASIC)
    if "FMOD_VECTOR" in c_type:
        return "3D vector"
    if c_type.startswith("FMOD_") and c_type not in BASIC_NUMERIC_TYPES:
        readable = _class_to_readable(c_type)
        return f"{readable.capitalize()} value"

    words = field_lower.replace("_", " ")
    return f"{words.capitalize()} value"


def get_input_args(method: ParsedMethod, skip_self: bool = False) -> list[Any]:
    args = method.args[1:] if skip_self else method.args
    return [arg for arg in args if arg.usage in INPUT_USAGES]


def get_output_args(method: ParsedMethod) -> list[Any]:
    return [arg for arg in method.args if arg.usage in OUTPUT_USAGES]


def get_arg_type_info(arg: Any) -> tuple[str, int]:
    if arg.usage == "output_ptr":
        child = getattr(arg.type, "child", None)
        if child is not None:
            return child.c_type, child.type
    return arg.type.c_type, arg.type.type


def write_script_api_file(
    output_path: str,
    enums: list[str],
    structs: list[ParsedStruct],
    global_functions: list[tuple[int, str, ParsedMethod]],
) -> None:
    env = Environment(
        loader=FileSystemLoader(str(_SCRIPT_DIR / "templates")),
        autoescape=False,
        trim_blocks=True,
        lstrip_blocks=True,
    )

    env.globals["c_type_to_lua_type"] = convert_c_type_to_lua_type
    env.globals["get_param_description"] = generate_parameter_description
    env.globals["get_input_args"] = get_input_args
    env.globals["get_output_args"] = get_output_args
    env.globals["get_arg_type_info"] = get_arg_type_info
    env.globals["generate_func_desc"] = generate_function_description
    env.globals["generate_global_func_desc"] = generate_global_func_description
    env.globals["get_class_description"] = get_class_description
    env.globals["get_class_notes"] = get_class_notes
    env.globals["generate_return_desc"] = generate_return_description
    env.globals["generate_enum_desc"] = generate_enum_description
    env.globals["is_sentinel_enum"] = is_sentinel_enum
    env.globals["generate_field_desc"] = generate_field_description

    template = env.get_template("fmod_script_api_template.yaml")

    rendered_output = template.render(
        enums=enums,
        structs=structs,
        global_functions=global_functions,
    )

    with open(output_path, "w") as output_file:
        output_file.write(rendered_output)

    log.info("Generated %s", output_path)
