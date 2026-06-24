import os
import tempfile
import unittest
from types import SimpleNamespace

from api_from_bindings import (
    TYPE_BASIC,
    TYPE_CLASS,
    TYPE_POINTER,
    TYPE_STRUCT,
    convert_c_type_to_lua_type,
    convert_to_snake_case,
    generate_parameter_description,
    get_arg_type_info,
    get_input_args,
    get_output_args,
    write_script_api_file,
)

SNAKE_CASE_CASES = [
    ("FMOD_SYSTEM", "fmod_system"),
    ("FMOD_STUDIO_SYSTEM", "fmod_studio_system"),
    ("FMOD_System_CreateSound", "fmod_system_create_sound"),
    ("FMOD_3D", "fmod_3d"),
    ("FMOD_3D_Attributes", "fmod_3d_attributes"),
    ("FMOD_STUDIO_EVENTINSTANCE", "fmod_studio_eventinstance"),
    ("SetVolume", "set_volume"),
    ("Get3DAttributes", "get_3d_attributes"),
    ("", ""),
    ("A", "a"),
]

TYPE_MAPPING_CASES = [
    # Basic numeric types
    ("int", TYPE_BASIC, "number"),
    ("float", TYPE_BASIC, "number"),
    ("double", TYPE_BASIC, "number"),
    ("short", TYPE_BASIC, "number"),
    ("long", TYPE_BASIC, "number"),
    ("unsigned int", TYPE_BASIC, "number"),
    ("unsigned char", TYPE_BASIC, "number"),
    # Bool
    ("FMOD_BOOL", TYPE_BASIC, "boolean"),
    # Enums map to number
    ("FMOD_MODE", TYPE_BASIC, "number"),
    ("FMOD_RESULT", TYPE_BASIC, "number"),
    # Pointer types
    ("char*", TYPE_POINTER, "string"),
    ("int*", TYPE_POINTER, "userdata"),
    ("float*", TYPE_POINTER, "userdata"),
    # Class types -> fmod.* namespace
    ("FMOD_SYSTEM", TYPE_CLASS, "fmod.system"),
    ("FMOD_SOUND", TYPE_CLASS, "fmod.sound"),
    ("FMOD_STUDIO_SYSTEM", TYPE_CLASS, "fmod.studio.system"),
    ("FMOD_STUDIO_EVENTDESCRIPTION", TYPE_CLASS, "fmod.studio.eventdescription"),
    # Struct types
    ("UNKNOWN_TYPE", TYPE_STRUCT, "userdata"),
    # FMOD_VECTOR is always vector3 regardless of type enum
    ("FMOD_VECTOR", TYPE_BASIC, "vector3"),
    ("FMOD_VECTOR", TYPE_POINTER, "vector3"),
    ("FMOD_VECTOR", TYPE_STRUCT, "vector3"),
    ("FMOD_VECTOR", TYPE_CLASS, "vector3"),
]

PARAM_DESCRIPTION_CASES = [
    # Exact matches
    ("system", "FMOD system handle"),
    ("sound", "Sound handle"),
    ("channel", "Channel handle"),
    ("filename", "File path"),
    ("name", "Name or path"),
    ("length", "Length value"),
    ("size", "Size in bytes"),
    ("mode", "Mode flags"),
    ("volume", "Volume level (0.0 to 1.0)"),
    ("position", "Position value"),
    ("paused", "Paused state"),
    ("index", "Index value"),
    # Case insensitive
    ("SYSTEM", "FMOD system handle"),
    ("Volume", "Volume level (0.0 to 1.0)"),
    # Substring matching
    ("sound_name", "Sound handle"),
    ("my_system", "FMOD system handle"),
    ("buffer_size", "Size in bytes"),
    # Unknown falls back to humanized name
    ("unknown_param", "Unknown param"),
    ("xyz", "xyz"),
]


class TestConvertToSnakeCase(unittest.TestCase):
    def test_cases(self):
        for input_str, expected in SNAKE_CASE_CASES:
            with self.subTest(input=input_str):
                self.assertEqual(convert_to_snake_case(input_str), expected)


class TestConvertCTypeToLuaType(unittest.TestCase):
    def test_cases(self):
        for c_type, type_enum, expected in TYPE_MAPPING_CASES:
            with self.subTest(c_type=c_type, type_enum=type_enum):
                self.assertEqual(convert_c_type_to_lua_type(c_type, type_enum), expected)

    def test_unknown_type_enum(self):
        self.assertEqual(convert_c_type_to_lua_type("int", 999), "any")


class TestGenerateParameterDescription(unittest.TestCase):
    def test_cases(self):
        for param, expected in PARAM_DESCRIPTION_CASES:
            with self.subTest(param=param):
                self.assertEqual(generate_parameter_description(param), expected)

    def test_exact_match_beats_substring(self):
        self.assertEqual(generate_parameter_description("name"), "Name or path")
        self.assertEqual(generate_parameter_description("filename"), "File path")


def _make_arg(name, usage, c_type="int", type_enum=TYPE_BASIC, child=None):
    arg = SimpleNamespace(
        name=name,
        usage=usage,
        type=SimpleNamespace(c_type=c_type, type=type_enum, child=child),
    )
    return arg


def _make_method(name, args, generated=True):
    return SimpleNamespace(name=name, args=args, generated=generated)


class TestGetInputArgs(unittest.TestCase):
    def test_filters_input_usages(self):
        args = [
            _make_arg("self", "input", "FMOD_SYSTEM*", TYPE_POINTER),
            _make_arg("volume", "input"),
            _make_arg("result", "output"),
        ]
        method = _make_method("FMOD_System_SetVolume", args)
        inputs = get_input_args(method, skip_self=True)
        self.assertEqual(len(inputs), 1)
        self.assertEqual(inputs[0].name, "volume")

    def test_skip_self_false(self):
        args = [
            _make_arg("self", "input"),
            _make_arg("value", "input_ptr"),
        ]
        method = _make_method("Test", args)
        inputs = get_input_args(method, skip_self=False)
        self.assertEqual(len(inputs), 2)

    def test_includes_input_deref(self):
        args = [_make_arg("data", "input_deref")]
        method = _make_method("Test", args)
        self.assertEqual(len(get_input_args(method)), 1)


class TestGetOutputArgs(unittest.TestCase):
    def test_filters_output_usages(self):
        args = [
            _make_arg("self", "input"),
            _make_arg("volume", "output"),
            _make_arg("ptr", "output_ptr"),
        ]
        method = _make_method("Test", args)
        outputs = get_output_args(method)
        self.assertEqual(len(outputs), 2)
        self.assertEqual(outputs[0].name, "volume")
        self.assertEqual(outputs[1].name, "ptr")


class TestGetArgTypeInfo(unittest.TestCase):
    def test_regular_arg(self):
        arg = _make_arg("volume", "input", "float", TYPE_BASIC)
        c_type, type_enum = get_arg_type_info(arg)
        self.assertEqual(c_type, "float")
        self.assertEqual(type_enum, TYPE_BASIC)

    def test_output_ptr_uses_child(self):
        child = SimpleNamespace(c_type="FMOD_SOUND*", type=TYPE_CLASS)
        arg = _make_arg("sound", "output_ptr", "FMOD_SOUND**", TYPE_POINTER, child=child)
        c_type, type_enum = get_arg_type_info(arg)
        self.assertEqual(c_type, "FMOD_SOUND*")
        self.assertEqual(type_enum, TYPE_CLASS)


class TestWriteScriptApiFile(unittest.TestCase):
    def test_generates_valid_yaml(self):
        fd, path = tempfile.mkstemp(suffix=".yaml")
        os.close(fd)
        try:
            write_script_api_file(path, ["OK", "ERR_INVALID_HANDLE"], [], [])
            with open(path) as f:
                content = f.read()
            self.assertIn("fmod", content)
            self.assertIn("OK", content)
            self.assertIn("ERR_INVALID_HANDLE", content)
        finally:
            os.unlink(path)


if __name__ == "__main__":
    unittest.main()
