import re
from typing import List, Tuple, Any, Protocol
from jinja2 import Environment, FileSystemLoader


class ParsedStruct(Protocol):
    name: str
    is_class: bool
    methods: List[Tuple[str, Any]]
    properties: List[Any]


class ParsedTypeDecl(Protocol):
    c_type: str
    type: int


class ParsedMethod(Protocol):
    name: str
    args: List[Any]
    generated: bool


TYPE_BASIC = 1
TYPE_STRUCT = 2
TYPE_CLASS = 3
TYPE_POINTER = 4

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
    "name": "Name or path",
    "filename": "Name or path",
    "length": "Length or size value",
    "size": "Length or size value",
    "mode": "Mode flags",
    "volume": "Volume level (0.0 to 1.0)",
    "position": "Position value",
    "paused": "Paused state",
    "index": "Index value",
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
    valid_pattern = re.compile(r"^_*(IDs|[A-Z][a-z]+|[A-Z0-9]+(?![a-z]))")
    components = []
    remaining = text
    while True:
        match = valid_pattern.match(remaining)
        if match is None:
            break
        components.append(match.group(1).lower())
        remaining = remaining[match.end():]
    return "_".join(components)


def convert_c_type_to_lua_type(c_type: str, type_enum: int) -> str:
    handler = TYPE_HANDLERS.get(type_enum)
    if handler:
        return handler(c_type)
    return "any"


def generate_parameter_description(param_name: str, function_name: str) -> str:
    param_lower = param_name.lower()
    
    if param_lower in PARAM_DESCRIPTIONS:
        return PARAM_DESCRIPTIONS[param_lower]
    
    for key, description in PARAM_DESCRIPTIONS.items():
        if key in param_lower:
            return description
    
    return param_name


def get_input_args(method: ParsedMethod, skip_self: bool = False) -> List[Any]:
    args = method.args[1:] if skip_self else method.args
    return [arg for arg in args if arg.usage in INPUT_USAGES]


def get_output_args(method: ParsedMethod) -> List[Any]:
    return [arg for arg in method.args if arg.usage in OUTPUT_USAGES]


def get_arg_type_info(arg: Any) -> Tuple[str, int]:
    if arg.usage == "output_ptr":
        child = getattr(arg.type, "child", None)
        if child is not None:
            return child.c_type, child.type
    return arg.type.c_type, arg.type.type


def write_script_api_file(
    output_path: str,
    enums: List[str],
    structs: List[ParsedStruct],
    global_functions: List[Tuple[int, str, ParsedMethod]]
) -> None:
    env = Environment(
        loader=FileSystemLoader('.'),
        autoescape=False,
        trim_blocks=True,
        lstrip_blocks=True,
    )
    
    env.globals['c_type_to_lua_type'] = convert_c_type_to_lua_type
    env.globals['get_param_description'] = generate_parameter_description
    env.globals['get_input_args'] = get_input_args
    env.globals['get_output_args'] = get_output_args
    env.globals['get_arg_type_info'] = get_arg_type_info
    
    template = env.get_template('fmod_script_api_template.yaml')
    
    rendered_output = template.render(
        enums=enums,
        structs=structs,
        global_functions=global_functions,
    )
    
    with open(output_path, 'w') as output_file:
        output_file.write(rendered_output)
    
    print(f"Generated {output_path}")
