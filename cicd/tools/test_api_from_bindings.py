import unittest
from bridge.api_from_bindings import (
    convert_to_snake_case,
    convert_c_type_to_lua_type,
    generate_parameter_description,
    TYPE_BASIC,
    TYPE_STRUCT,
    TYPE_CLASS,
    TYPE_POINTER,
)


class TestConvertToSnakeCase(unittest.TestCase):
    def test_simple_identifier(self):
        self.assertEqual(convert_to_snake_case("FMOD_SYSTEM"), "fmod_system")
    
    def test_mixed_case(self):
        self.assertEqual(convert_to_snake_case("FMOD_System_CreateSound"), "fmod_system_create_sound")
    
    def test_numbers(self):
        self.assertEqual(convert_to_snake_case("FMOD_3D"), "fmod_3d")
    
    def test_studio_prefix(self):
        self.assertEqual(convert_to_snake_case("FMOD_STUDIO_SYSTEM"), "fmod_studio_system")


class TestConvertCTypeToLuaType(unittest.TestCase):
    def test_basic_numeric_types(self):
        self.assertEqual(convert_c_type_to_lua_type("int", TYPE_BASIC), "number")
        self.assertEqual(convert_c_type_to_lua_type("float", TYPE_BASIC), "number")
        self.assertEqual(convert_c_type_to_lua_type("double", TYPE_BASIC), "number")
        self.assertEqual(convert_c_type_to_lua_type("short", TYPE_BASIC), "number")
        self.assertEqual(convert_c_type_to_lua_type("long", TYPE_BASIC), "number")
    
    def test_bool_type(self):
        self.assertEqual(convert_c_type_to_lua_type("FMOD_BOOL", TYPE_BASIC), "boolean")
    
    def test_enum_types(self):
        self.assertEqual(convert_c_type_to_lua_type("FMOD_MODE", TYPE_BASIC), "number")
    
    def test_pointer_to_char(self):
        self.assertEqual(convert_c_type_to_lua_type("char*", TYPE_POINTER), "string")
    
    def test_pointer_to_other(self):
        self.assertEqual(convert_c_type_to_lua_type("int*", TYPE_POINTER), "userdata")
    
    def test_fmod_struct(self):
        self.assertEqual(convert_c_type_to_lua_type("FMOD_SYSTEM", TYPE_CLASS), "fmod.system")
    
    def test_fmod_studio_struct(self):
        self.assertEqual(convert_c_type_to_lua_type("FMOD_STUDIO_SYSTEM", TYPE_CLASS), "fmod.studio.system")
    
    def test_regular_struct(self):
        self.assertEqual(convert_c_type_to_lua_type("FMOD_VECTOR", TYPE_STRUCT), "vector3")
    
    def test_unknown_struct(self):
        self.assertEqual(convert_c_type_to_lua_type("UNKNOWN_TYPE", TYPE_STRUCT), "userdata")
    
    def test_vector_types(self):
        self.assertEqual(convert_c_type_to_lua_type("FMOD_VECTOR", TYPE_BASIC), "vector3")
        self.assertEqual(convert_c_type_to_lua_type("FMOD_VECTOR", TYPE_POINTER), "vector3")
        self.assertEqual(convert_c_type_to_lua_type("FMOD_VECTOR", TYPE_STRUCT), "vector3")
        self.assertEqual(convert_c_type_to_lua_type("FMOD_VECTOR", TYPE_CLASS), "vector3")


class TestGenerateParameterDescription(unittest.TestCase):
    def test_system_parameter(self):
        desc = generate_parameter_description("system", "FMOD_System_CreateSound")
        self.assertEqual(desc, "FMOD system handle")
    
    def test_sound_parameter(self):
        desc = generate_parameter_description("sound", "FMOD_System_PlaySound")
        self.assertEqual(desc, "Sound handle")
    
    def test_channel_parameter(self):
        desc = generate_parameter_description("channel", "FMOD_Channel_SetVolume")
        self.assertEqual(desc, "Channel handle")
    
    def test_filename_parameter(self):
        desc = generate_parameter_description("filename", "FMOD_System_CreateSound")
        self.assertEqual(desc, "Name or path")
    
    def test_length_parameter(self):
        desc = generate_parameter_description("length", "FMOD_Sound_GetLength")
        self.assertEqual(desc, "Length or size value")
    
    def test_mode_parameter(self):
        desc = generate_parameter_description("mode", "FMOD_System_CreateSound")
        self.assertEqual(desc, "Mode flags")
    
    def test_volume_parameter(self):
        desc = generate_parameter_description("volume", "FMOD_Channel_SetVolume")
        self.assertEqual(desc, "Volume level (0.0 to 1.0)")
    
    def test_position_parameter(self):
        desc = generate_parameter_description("position", "FMOD_Channel_Set3DAttributes")
        self.assertEqual(desc, "Position value")
    
    def test_paused_parameter(self):
        desc = generate_parameter_description("paused", "FMOD_Channel_SetPaused")
        self.assertEqual(desc, "Paused state")
    
    def test_index_parameter(self):
        desc = generate_parameter_description("index", "FMOD_System_GetChannel")
        self.assertEqual(desc, "Index value")
    
    def test_unknown_parameter(self):
        desc = generate_parameter_description("unknown_param", "FMOD_SomeFunction")
        self.assertEqual(desc, "unknown_param")
    
    def test_case_insensitive(self):
        desc = generate_parameter_description("SYSTEM", "FMOD_System_CreateSound")
        self.assertEqual(desc, "FMOD system handle")
    
    def test_substring_matching(self):
        # Test that substring matching works (e.g., "sound" in "sound_name")
        desc = generate_parameter_description("sound_name", "FMOD_System_CreateSound")
        self.assertEqual(desc, "Sound handle")
        
        desc = generate_parameter_description("my_system", "FMOD_System_CreateSound")
        self.assertEqual(desc, "FMOD system handle")
        
        desc = generate_parameter_description("channel_index", "FMOD_System_GetChannel")
        self.assertEqual(desc, "Channel handle")  # "channel" should match first
        
        desc = generate_parameter_description("buffer_size", "FMOD_SomeFunction")
        self.assertEqual(desc, "Length or size value")  # "size" should match
    
    def test_exact_match_precedence(self):
        # Test that exact matches take precedence over substring matches
        desc = generate_parameter_description("name", "FMOD_System_CreateSound")
        self.assertEqual(desc, "Name or path")  # Exact match
        
        desc = generate_parameter_description("filename", "FMOD_System_CreateSound")
        self.assertEqual(desc, "Name or path")  # Exact match
        
        # These should match as substrings
        desc = generate_parameter_description("soundname", "FMOD_System_CreateSound")
        self.assertEqual(desc, "Sound handle")  # "sound" substring match


if __name__ == '__main__':
    unittest.main()

