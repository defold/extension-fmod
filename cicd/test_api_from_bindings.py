import unittest

from api_from_bindings import (
    TYPE_BASIC,
    TYPE_CLASS,
    TYPE_POINTER,
    TYPE_STRUCT,
    convert_c_type_to_lua_type,
    convert_to_snake_case,
    generate_parameter_description,
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
    ("filename", "Name or path"),
    ("name", "Name or path"),
    ("length", "Length or size value"),
    ("size", "Length or size value"),
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
    ("buffer_size", "Length or size value"),
    # Unknown falls back to raw name
    ("unknown_param", "unknown_param"),
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
                self.assertEqual(
                    generate_parameter_description(param, "FMOD_Unused"), expected
                )

    def test_exact_match_beats_substring(self):
        self.assertEqual(generate_parameter_description("name", ""), "Name or path")
        self.assertEqual(generate_parameter_description("filename", ""), "Name or path")


if __name__ == "__main__":
    unittest.main()
