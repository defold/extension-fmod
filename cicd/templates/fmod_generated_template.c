#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "fmod_ext.h"
#include "fmod_errors.h"

/* Error handling */

static int FMODExt_registry_FMOD_RESULT = LUA_REFNIL;
inline static void throwError(FMOD_RESULT res, lua_State* L) {
    lua_pushstring(L, FMOD_ErrorString(res));
    lua_rawgeti(L, LUA_REGISTRYINDEX, FMODExt_registry_FMOD_RESULT);
    lua_pushvalue(L, -2);
    lua_pushnumber(L, res);
    lua_rawset(L, -3);
    lua_pop(L, 1);
    lua_error(L);
}
#define errCheckBegin(expr) do { FMOD_RESULT res = (expr); if (res != FMOD_OK) {
#define errCheckEnd throwError(res, L); } } while (0)
#define errCheck(res) errCheckBegin(res); errCheckEnd

/* Basic types */

#define optional(L, index, typename, x) (lua_isnoneornil(L, index) ? ((typename)0) : (x))

#define FMODExt_push_char(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_char(L, index) ((char)luaL_checknumber(L, index))
#define FMODExt_push_short(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_short(L, index) ((short)luaL_checknumber(L, index))
#define FMODExt_push_int(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_int(L, index) ((int)luaL_checknumber(L, index))
#define FMODExt_push_unsigned_char(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_unsigned_char(L, index) ((unsigned char)luaL_checknumber(L, index))
#define FMODExt_push_unsigned_short(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_unsigned_short(L, index) ((unsigned short)luaL_checknumber(L, index))
#define FMODExt_push_unsigned_int(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_unsigned_int(L, index) ((unsigned int)luaL_checknumber(L, index))
#define FMODExt_push_float(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_float(L, index) ((float)luaL_checknumber(L, index))
#define FMODExt_push_double(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_double(L, index) ((double)luaL_checknumber(L, index))
#define FMODExt_push_FMOD_BOOL(L, x) lua_pushboolean(L, (x))
#define FMODExt_check_FMOD_BOOL(L, index) (luaL_checktype(L, index, LUA_TBOOLEAN), (FMOD_BOOL)lua_toboolean(L, index))

#define FMODExt_push_ptr_char _FMODExt_push_ptr_char
inline static void _FMODExt_push_ptr_char(lua_State *L, const char * x) {
    if (x) {
        lua_pushstring(L, x);
    } else {
        lua_pushnil(L);
    }
}
#define FMODExt_check_ptr_char(L, index) luaL_checkstring(L, index)

#define FMODExt_check_FMOD_VECTOR FMODExt_dmScript_CheckVector3
#define FMODExt_push_FMOD_VECTOR _FMODExt_push_FMOD_VECTOR
inline static void _FMODExt_push_FMOD_VECTOR(lua_State *L, FMOD_VECTOR vec) {
    FMODExt_dmScript_PushVector3(L, vec.x, vec.y, vec.z);
}
#define FMODExt_push_ptr_FMOD_VECTOR _FMODExt_push_ptr_FMOD_VECTOR
inline static void _FMODExt_push_ptr_FMOD_VECTOR(lua_State *L, const FMOD_VECTOR * vec) {
    FMODExt_dmScript_PushVector3(L, vec->x, vec->y, vec->z);
}

{% for enum in enum_types %}
#define FMODExt_push_{{ enum }}(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_{{ enum }}(L, index) (({{ enum }})luaL_checknumber(L, index))
{% endfor %}

/* Structs and Classes */

static void * pushStruct(lua_State *L, const void * structData, size_t structSize, int registryIndex) {
    void * userdata = lua_newuserdata(L, structSize);
    if (structData) {
        memcpy(userdata, structData, structSize);
    } else {
        memset(userdata, 0, structSize);
    }

    lua_rawgeti(L, LUA_REGISTRYINDEX, registryIndex);
    lua_setmetatable(L, -2);
    return userdata;
}

static void * checkStruct(lua_State *L, int index, int registryIndex, const char * structName) {
    void * userdata = lua_touserdata(L, index);
    lua_rawgeti(L, LUA_REGISTRYINDEX, registryIndex);
    if (userdata == NULL || !lua_getmetatable(L, index) || !lua_rawequal(L, -1, -2)) {
        luaL_typerror(L, index, structName);
    }
    lua_pop(L, 2);
    return userdata;
}

static void * pushClass(lua_State *L, void * instance, int registryIndex) {
    void ** userdata = (void**)lua_newuserdata(L, sizeof(void*));
    *userdata = instance;

    lua_rawgeti(L, LUA_REGISTRYINDEX, registryIndex);
    lua_setmetatable(L, -2);
    return instance;
}

static int FMODExt_registry_refcount = LUA_REFNIL;
static int FMODExt_registry_rolloff = LUA_REFNIL;

static void * pushClassRefCount(lua_State *L, void * instance, int registryIndex) {
    pushClass(L, instance, registryIndex);

    /* Increment the ref count */
    lua_rawgeti(L, LUA_REGISTRYINDEX, FMODExt_registry_refcount);
    lua_pushlightuserdata(L, instance);
    lua_rawget(L, -2);
    lua_Number refcount = lua_tonumber(L, -1); /* If the value is nil, lua_tonumber returns 0 */
    refcount += 1;
    lua_pushlightuserdata(L, instance);
    lua_pushnumber(L, refcount);
    lua_rawset(L, -4);
    lua_pop(L, 2);

    return instance;
}

#define checkClass(L, index, registryIndex, structName) (*((void**)checkStruct(L, index, registryIndex, structName)))

static int classGC(lua_State *L) {
    void * instance = *((void**)lua_touserdata(L, 1));

    lua_rawgeti(L, LUA_REGISTRYINDEX, FMODExt_registry_refcount);
    lua_pushlightuserdata(L, instance);
    lua_rawget(L, -2);

    if (lua_isnil(L, -1)) {
        lua_pop(L, 2);
        return 0;
    }

    lua_Number refcount = lua_tonumber(L, -1);
    refcount -= 1;
    lua_pushlightuserdata(L, instance);
    lua_pushnumber(L, refcount);
    lua_rawset(L, -4);
    lua_pop(L, 2);

    if (refcount == 0) {
        lua_pushvalue(L, lua_upvalueindex(1));
        lua_pushvalue(L, 1);
        lua_pcall(L, 1, 0, 0);
    }

    return 0;
}

{% for struct in structs
    %}static int FMODExt_registry_{{ struct.name }} = LUA_REFNIL;
{% endfor %}

/* 64-bit values */

static int FMODExt_registry_long_long = LUA_REFNIL;
static int FMODExt_registry_unsigned_long_long = LUA_REFNIL;

#define FMODExt_check_long_long(L, index) checkLongLong(L, index, FMODExt_registry_long_long, "s64")
#define FMODExt_check_unsigned_long_long(L, index) ((unsigned long long)checkLongLong(L, index, FMODExt_registry_unsigned_long_long, "u64"))
#define FMODExt_push_long_long(L, x) pushLongLong(L, (long long)(x), FMODExt_registry_long_long)
#define FMODExt_push_unsigned_long_long(L, x) pushLongLong(L, (long long)(x), FMODExt_registry_unsigned_long_long)

long long checkLongLong(lua_State *L, int index, int registryIndex, const char * structName) {
    if (lua_type(L, index) == LUA_TNUMBER) {
        return (long long)lua_tonumber(L, index);
    }
    return *((long long*)checkStruct(L, index, registryIndex, structName));
}

static void pushLongLong(lua_State *L, long long value, int registryIndex) {
    long long* userdata = (long long*)lua_newuserdata(L, sizeof(long long));
    *userdata = value;
    lua_rawgeti(L, LUA_REGISTRYINDEX, registryIndex);
    lua_setmetatable(L, -2);
}

static int longLongConstructor(lua_State *L) {
    long long value;
    lua_Number arg1 = luaL_checknumber(L, 1);
    if (lua_isnoneornil(L, 2)) {
        value = (long long)arg1;
    } else {
        lua_Number arg2 = luaL_checknumber(L, 2);
        value = ((long long)((long)arg1)) | (((long long)((long)arg2)) << 32);
    }
    pushLongLong(L, value, (int)lua_tonumber(L, lua_upvalueindex(1)));
    return 1;
}

static int longLongGetValue(lua_State *L) {
    long long value = FMODExt_check_long_long(L, 1);
    lua_pushnumber(L, (lua_Number)value);
    return 1;
}

static int unsignedLongLongGetValue(lua_State *L) {
    unsigned long long value = FMODExt_check_unsigned_long_long(L, 1);
    lua_pushnumber(L, (lua_Number)value);
    return 1;
}

static int longLongGetLow(lua_State *L) {
    unsigned long long value = (unsigned long long)FMODExt_check_long_long(L, 1);
    lua_pushnumber(L, (unsigned int)value);
    return 1;
}

static int unsignedLongLongGetLow(lua_State *L) {
    unsigned long long value = (unsigned long long)FMODExt_check_unsigned_long_long(L, 1);
    lua_pushnumber(L, (unsigned int)value);
    return 1;
}

static int longLongGetHigh(lua_State *L) {
    unsigned long long value = (unsigned long long)FMODExt_check_long_long(L, 1);
    lua_pushnumber(L, (unsigned int)(value >> 32));
    return 1;
}

static int unsignedLongLongGetHigh(lua_State *L) {
    unsigned long long value = (unsigned long long)FMODExt_check_unsigned_long_long(L, 1);
    lua_pushnumber(L, (unsigned int)(value >> 32));
    return 1;
}

static int longLongToString(lua_State *L) {
    char s[22];
    snprintf(s, 22, "%lld", FMODExt_check_long_long(L, 1));
    lua_pushstring(L, s);
    return 1;
}

static int unsignedLongLongToString(lua_State *L) {
    char s[22];
    snprintf(s, 22, "%llu", FMODExt_check_unsigned_long_long(L, 1));
    lua_pushstring(L, s);
    return 1;
}

/* Struct properties */

#define declarePropertyGetter(typename, type) \
    static int CONCAT(FMODExt_propertyGet_, typename)(lua_State *L) { \
        char *base = (char*)lua_touserdata(L, 1); \
        size_t offset = (size_t)lua_touserdata(L, lua_upvalueindex(1)); \
        type *member = (type*)(base + offset); \
        CONCAT(FMODExt_push_, typename)(L, *member); \
        return 1; \
    }

#define declarePropertyGetterPtr(typename, type) \
    static int CONCAT(FMODExt_propertyGet_, typename)(lua_State *L) { \
        char *base = (char*)lua_touserdata(L, 1); \
        size_t offset = (size_t)lua_touserdata(L, lua_upvalueindex(1)); \
        type *member = (type*)(base + offset); \
        CONCAT(FMODExt_push_ptr_, typename)(L, member); \
        return 1; \
    }

#define declarePropertySetter(typename, type) \
    static int CONCAT(FMODExt_propertySet_, typename)(lua_State *L) { \
        char *base = (char*)lua_touserdata(L, 1); \
        size_t offset = (size_t)lua_touserdata(L, lua_upvalueindex(1)); \
        type *member = (type*)(base + offset); \
        *member = CONCAT(FMODExt_check_, typename)(L, 2); \
        return 0; \
    } \

#define declarePropertySetterPtr(typename, type) \
    static int CONCAT(FMODExt_propertySet_, typename)(lua_State *L) { \
        char *base = (char*)lua_touserdata(L, 1); \
        size_t offset = (size_t)lua_touserdata(L, lua_upvalueindex(1)); \
        type *member = (type*)(base + offset); \
        *member = *CONCAT(FMODExt_check_ptr_, typename)(L, 2); \
        return 0; \
    } \

declarePropertyGetter(char, char);
declarePropertySetter(char, char);
declarePropertyGetter(short, short);
declarePropertySetter(short, short);
declarePropertyGetter(int, int);
declarePropertySetter(int, int);
declarePropertyGetter(long_long, long long);
declarePropertySetter(long_long, long long);
declarePropertyGetter(unsigned_char, unsigned char);
declarePropertySetter(unsigned_char, unsigned char);
declarePropertyGetter(unsigned_short, unsigned short);
declarePropertySetter(unsigned_short, unsigned short);
declarePropertyGetter(unsigned_int, unsigned int);
declarePropertySetter(unsigned_int, unsigned int);
declarePropertyGetter(unsigned_long_long, unsigned long long);
declarePropertySetter(unsigned_long_long, unsigned long long);
declarePropertyGetter(float, float);
declarePropertySetter(float, float);
declarePropertyGetter(double, double);
declarePropertySetter(double, double);
declarePropertyGetter(FMOD_BOOL, FMOD_BOOL);
declarePropertySetter(FMOD_BOOL, FMOD_BOOL);
declarePropertyGetter(ptr_char, char*);
declarePropertyGetterPtr(FMOD_VECTOR, FMOD_VECTOR);
declarePropertySetter(FMOD_VECTOR, FMOD_VECTOR);

{% for enum in enum_types %}
#define FMODExt_propertyGet_{{ enum }} FMODExt_propertyGet_int
#define FMODExt_propertySet_{{ enum }} FMODExt_propertySet_int
{% endfor %}

{% for struct in structs %}
#ifndef FMODExt_push_ptr_{{ struct.name }}
{% if struct.ref_counted %}#define FMODExt_push_ptr_{{ struct.name }}(L, instance) (({{ struct.name }}*)pushClassRefCount(L, instance, FMODExt_registry_{{ struct.name }}))
{% elif struct.is_class %}#define FMODExt_push_ptr_{{ struct.name }}(L, instance) (({{ struct.name }}*)pushClass(L, instance, FMODExt_registry_{{ struct.name }}))
{% else %}#define FMODExt_push_ptr_{{ struct.name }}(L, structData) (({{ struct.name }}*)pushStruct(L, structData, sizeof({{ struct.name }}), FMODExt_registry_{{ struct.name }}))
{% endif %}#endif
#ifndef FMODExt_check_ptr_{{ struct.name }}
{% if struct.is_class %}#define FMODExt_check_ptr_{{ struct.name }}(L, index) (({{ struct.name }}*)checkClass(L, index, FMODExt_registry_{{ struct.name }}, "{{ struct.name }}"))
{% else %}#define FMODExt_check_ptr_{{ struct.name }}(L, index) (({{ struct.name }}*)checkStruct(L, index, FMODExt_registry_{{ struct.name }}, "{{ struct.name }}"))
{% endif %}#endif
#ifdef FMODExt_propertyOverride_{{ struct.name }}
FMODExt_propertyOverride_{{ struct.name }}
#else
declarePropertyGetter(ptr_{{ struct.name }}, {{ struct.name }}*);
{% if not struct.is_class %}declarePropertyGetterPtr({{ struct.name }}, {{ struct.name }});
declarePropertySetterPtr({{ struct.name }}, {{ struct.name }});
{% endif %}#endif
{% endfor %}

static int structConstructor(lua_State *L) {
    int registryIndex = (int)lua_tonumber(L, lua_upvalueindex(1));
    size_t structSize = (size_t)lua_tonumber(L, lua_upvalueindex(2));
    pushStruct(L, NULL, structSize, registryIndex);
    return 1;
}

static int structIndexMetamethod(lua_State *L) {
    int metatableIndex = lua_upvalueindex(1);

    lua_pushvalue(L, 2);
    lua_rawget(L, metatableIndex);

    if (!lua_isnil(L, -1)) { return 1; }

    lua_pushstring(L, "__fieldget");
    lua_rawget(L, metatableIndex);

    lua_pushvalue(L, 2);
    lua_rawget(L, -2);

    if (!lua_isfunction(L, -1)) { return 1; }

    lua_pushvalue(L, 1);
    lua_call(L, 1, 1);
    return 1;
}

static int structNewIndexMetamethod(lua_State *L) {
    int metatableIndex = lua_upvalueindex(1);

    lua_pushstring(L, "__fieldset");
    lua_rawget(L, metatableIndex);

    lua_pushvalue(L, 2);
    lua_rawget(L, -2);

    if (!lua_isfunction(L, -1)) {
        lua_getglobal(L, "tostring");
        lua_pushvalue(L, 2);
        if (lua_pcall(L, 1, 1, 0) == 0) {
            const char *key = lua_tostring(L, -1);
            luaL_error(L, "Cannot set invalid property \"%s\"", key);
        } else {
            luaL_error(L, "Cannot set invalid property");
        }
    }

    lua_pushvalue(L, 1);
    lua_pushvalue(L, 3);
    lua_call(L, 2, 0);
    return 0;
}

static int structEqualityMetamethod(lua_State *L) {
    size_t structSize = (size_t)lua_tonumber(L, lua_upvalueindex(1));
    const char * a = (const char*)lua_touserdata(L, 1);
    const char * b = (const char*)lua_touserdata(L, 2);
    lua_pushboolean(L, !memcmp(a, b, structSize));
    return 1;
}

/* Custom function definitions */

void * FMODExt_check_ptr_void_size(lua_State *L, int index, size_t *length) {
    if (lua_type(L, index) == LUA_TSTRING) {
        return (char*)lua_tolstring(L, index, length);
    }
    FMODExt_HBuffer buffer = FMODExt_dmScript_CheckBuffer(L, index);
    void *data;
    uint32_t size;
    if (FMODExt_dmBuffer_GetBytes(buffer, &data, &size)) {
        luaL_error(L, "dmBuffer::GetBytes failed");
    }
    *length = size;
    return data;
}

#define FMODExt_func_FMOD_Studio_System_LoadBankMemory _FMODExt_func_FMOD_Studio_System_LoadBankMemory
static int _FMODExt_func_FMOD_Studio_System_LoadBankMemory(lua_State *L) {
    FMOD_STUDIO_SYSTEM *system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    size_t length;
    const char *buffer = (const char*)FMODExt_check_ptr_void_size(L, 2, &length);
    FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMODExt_check_unsigned_int(L, 3);

    FMOD_STUDIO_BANK *bank;

    errCheck(FMOD_Studio_System_LoadBankMemory(system, buffer, (int)length, FMOD_STUDIO_LOAD_MEMORY, flags, &bank));

    FMODExt_push_ptr_FMOD_STUDIO_BANK(L, bank);
    return 1;
}

#define FMODExt_func_FMOD_Studio_System_SetListenerAttributes _FMODExt_func_FMOD_Studio_System_SetListenerAttributes
static int _FMODExt_func_FMOD_Studio_System_SetListenerAttributes(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_3D_ATTRIBUTES* attributes = FMODExt_check_ptr_FMOD_3D_ATTRIBUTES(L, 3);
    FMOD_VECTOR attenuationposition;
    int hasAttenuationPosition = !lua_isnoneornil(L, 4);
    if (hasAttenuationPosition) {
        attenuationposition = FMODExt_check_FMOD_VECTOR(L, 4);
    }
    errCheck(FMOD_Studio_System_SetListenerAttributes(system, index, attributes, hasAttenuationPosition ? &attenuationposition : NULL));
    return 0;
}

#define FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes _FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes
static int _FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_3D_ATTRIBUTES* attributes = FMODExt_check_ptr_FMOD_3D_ATTRIBUTES(L, 2);
    errCheck(FMOD_Studio_EventInstance_Set3DAttributes(eventinstance, attributes));
    return 0;
}

static void rolloff_store(lua_State *L, void *fmod_obj, FMOD_VECTOR *points) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, FMODExt_registry_rolloff);
    lua_pushlightuserdata(L, fmod_obj);
    lua_rawget(L, -2);
    if (!lua_isnil(L, -1)) {
        free(lua_touserdata(L, -1));
    }
    lua_pop(L, 1);
    lua_pushlightuserdata(L, fmod_obj);
    if (points) {
        lua_pushlightuserdata(L, points);
    } else {
        lua_pushnil(L);
    }
    lua_rawset(L, -3);
    lua_pop(L, 1);
}

static FMOD_VECTOR *rolloff_from_table(lua_State *L, int index, int *out_count) {
    luaL_checktype(L, index, LUA_TTABLE);
    int n = (int)lua_objlen(L, index);
    *out_count = n;
    if (n == 0) return NULL;
    FMOD_VECTOR *points = (FMOD_VECTOR*)malloc(sizeof(FMOD_VECTOR) * n);
    for (int i = 0; i < n; i++) {
        lua_rawgeti(L, index, i + 1);
        points[i] = FMODExt_check_FMOD_VECTOR(L, -1);
        lua_pop(L, 1);
    }
    return points;
}

static void rolloff_push_table(lua_State *L, FMOD_VECTOR *points, int numpoints) {
    lua_createtable(L, numpoints, 0);
    for (int i = 0; i < numpoints; i++) {
        FMODExt_push_FMOD_VECTOR(L, points[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

#define IMPL_Set3DCustomRolloff(name, CTYPE) \
    static int _FMODExt_func_FMOD_##name##_Set3DCustomRolloff(lua_State *L) { \
        FMOD_##CTYPE *obj = FMODExt_check_ptr_FMOD_##CTYPE(L, 1); \
        int numpoints; \
        FMOD_VECTOR *points = rolloff_from_table(L, 2, &numpoints); \
        errCheckBegin(FMOD_##name##_Set3DCustomRolloff(obj, points, numpoints)) { \
            free(points); \
        } errCheckEnd; \
        rolloff_store(L, obj, points); \
        return 0; \
    }

#define IMPL_Get3DCustomRolloff(name, CTYPE) \
    static int _FMODExt_func_FMOD_##name##_Get3DCustomRolloff(lua_State *L) { \
        FMOD_##CTYPE *obj = FMODExt_check_ptr_FMOD_##CTYPE(L, 1); \
        FMOD_VECTOR *points; \
        int numpoints; \
        errCheck(FMOD_##name##_Get3DCustomRolloff(obj, &points, &numpoints)); \
        rolloff_push_table(L, points, numpoints); \
        return 1; \
    }

#define FMODExt_func_FMOD_Sound_Set3DCustomRolloff _FMODExt_func_FMOD_Sound_Set3DCustomRolloff
IMPL_Set3DCustomRolloff(Sound, SOUND)
#define FMODExt_func_FMOD_Sound_Get3DCustomRolloff _FMODExt_func_FMOD_Sound_Get3DCustomRolloff
IMPL_Get3DCustomRolloff(Sound, SOUND)

#define FMODExt_func_FMOD_Channel_Set3DCustomRolloff _FMODExt_func_FMOD_Channel_Set3DCustomRolloff
IMPL_Set3DCustomRolloff(Channel, CHANNEL)
#define FMODExt_func_FMOD_Channel_Get3DCustomRolloff _FMODExt_func_FMOD_Channel_Get3DCustomRolloff
IMPL_Get3DCustomRolloff(Channel, CHANNEL)

#define FMODExt_func_FMOD_ChannelGroup_Set3DCustomRolloff _FMODExt_func_FMOD_ChannelGroup_Set3DCustomRolloff
IMPL_Set3DCustomRolloff(ChannelGroup, CHANNELGROUP)
#define FMODExt_func_FMOD_ChannelGroup_Get3DCustomRolloff _FMODExt_func_FMOD_ChannelGroup_Get3DCustomRolloff
IMPL_Get3DCustomRolloff(ChannelGroup, CHANNELGROUP)

#define FMODExt_func_FMOD_DSP_GetParameterFloat _FMODExt_func_FMOD_DSP_GetParameterFloat
static int _FMODExt_func_FMOD_DSP_GetParameterFloat(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    float value;
    char valuestr[FMOD_DSP_GETPARAM_VALUESTR_LENGTH];
    errCheck(FMOD_DSP_GetParameterFloat(dsp, index, &value, valuestr, FMOD_DSP_GETPARAM_VALUESTR_LENGTH));
    FMODExt_push_float(L, value);
    FMODExt_push_ptr_char(L, valuestr);
    return 2;
}

#define FMODExt_func_FMOD_DSP_GetParameterInt _FMODExt_func_FMOD_DSP_GetParameterInt
static int _FMODExt_func_FMOD_DSP_GetParameterInt(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    int value;
    char valuestr[FMOD_DSP_GETPARAM_VALUESTR_LENGTH];
    errCheck(FMOD_DSP_GetParameterInt(dsp, index, &value, valuestr, FMOD_DSP_GETPARAM_VALUESTR_LENGTH));
    FMODExt_push_int(L, value);
    FMODExt_push_ptr_char(L, valuestr);
    return 2;
}


#define FMODExt_func_FMOD_DSP_GetParameterBool _FMODExt_func_FMOD_DSP_GetParameterBool
static int _FMODExt_func_FMOD_DSP_GetParameterBool(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_BOOL value;
    char valuestr[FMOD_DSP_GETPARAM_VALUESTR_LENGTH];
    errCheck(FMOD_DSP_GetParameterBool(dsp, index, &value, valuestr, FMOD_DSP_GETPARAM_VALUESTR_LENGTH));
    FMODExt_push_FMOD_BOOL(L, value);
    FMODExt_push_ptr_char(L, valuestr);
    return 2;
}

#define FMODExt_func_FMOD_System_GetDriverInfo _FMODExt_func_FMOD_System_GetDriverInfo
static int _FMODExt_func_FMOD_System_GetDriverInfo(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int id = FMODExt_check_int(L, 2);
    char name[256];
    FMOD_GUID* guid = FMODExt_push_ptr_FMOD_GUID(L, NULL);
    int systemrate;
    FMOD_SPEAKERMODE speakermode;
    int speakermodechannels;
    errCheck(FMOD_System_GetDriverInfo(system, id, name, 256, guid, &systemrate, &speakermode, &speakermodechannels));
    lua_pushstring(L, name);
    lua_pushvalue(L, -2);
    FMODExt_push_int(L, systemrate);
    FMODExt_push_FMOD_SPEAKERMODE(L, speakermode);
    FMODExt_push_int(L, speakermodechannels);
    return 5;
}

#define FMODExt_func_FMOD_System_GetPluginInfo _FMODExt_func_FMOD_System_GetPluginInfo
static int _FMODExt_func_FMOD_System_GetPluginInfo(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int handle = FMODExt_check_unsigned_int(L, 2);
    FMOD_PLUGINTYPE plugintype;
    char name[256];
    unsigned int version;
    errCheck(FMOD_System_GetPluginInfo(system, handle, &plugintype, name, 256, &version));
    FMODExt_push_FMOD_PLUGINTYPE(L, plugintype);
    lua_pushstring(L, name);
    FMODExt_push_unsigned_int(L, version);
    return 3;
}

#define FMODExt_func_FMOD_System_GetRecordDriverInfo _FMODExt_func_FMOD_System_GetRecordDriverInfo
static int _FMODExt_func_FMOD_System_GetRecordDriverInfo(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int id = FMODExt_check_int(L, 2);
    char name[256];
    FMOD_GUID* guid = FMODExt_push_ptr_FMOD_GUID(L, NULL);
    int systemrate;
    FMOD_SPEAKERMODE speakermode;
    int speakermodechannels;
    unsigned int state;
    errCheck(FMOD_System_GetRecordDriverInfo(system, id, name, 256, guid, &systemrate, &speakermode, &speakermodechannels, &state));
    lua_pushstring(L, name);
    lua_pushvalue(L, -2);
    FMODExt_push_int(L, systemrate);
    FMODExt_push_FMOD_SPEAKERMODE(L, speakermode);
    FMODExt_push_int(L, speakermodechannels);
    FMODExt_push_unsigned_int(L, state);
    return 6;
}

#define nameGetter(fname, type1, size) static int CONCAT(_FMODExt_func_, fname)(lua_State *L) { \
    type1* arg1 = CONCAT(FMODExt_check_ptr_, type1)(L, 1); \
    char name[size]; \
    errCheck(fname(arg1, name, size)); \
    lua_pushstring(L, name); \
    return 1; \
}

#define FMODExt_func_FMOD_System_GetNetworkProxy _FMODExt_func_FMOD_System_GetNetworkProxy
nameGetter(FMOD_System_GetNetworkProxy, FMOD_SYSTEM, 512);

#define FMODExt_func_FMOD_Sound_GetName _FMODExt_func_FMOD_Sound_GetName
nameGetter(FMOD_Sound_GetName, FMOD_SOUND, 256);

#define FMODExt_func_FMOD_Sound_GetSyncPointInfo _FMODExt_func_FMOD_Sound_GetSyncPointInfo
static int _FMODExt_func_FMOD_Sound_GetSyncPointInfo(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_SYNCPOINT* point = FMODExt_check_ptr_FMOD_SYNCPOINT(L, 2);
    char name[256];
    unsigned int offset;
    unsigned int offsettype = FMODExt_check_unsigned_int(L, 3);
    errCheck(FMOD_Sound_GetSyncPointInfo(sound, point, name, 256, &offset, offsettype));
    lua_pushstring(L, name);
    FMODExt_push_unsigned_int(L, offset);
    return 2;
}

#define FMODExt_func_FMOD_ChannelGroup_GetName _FMODExt_func_FMOD_ChannelGroup_GetName
nameGetter(FMOD_ChannelGroup_GetName, FMOD_CHANNELGROUP, 256);

#define FMODExt_func_FMOD_SoundGroup_GetName _FMODExt_func_FMOD_SoundGroup_GetName
nameGetter(FMOD_SoundGroup_GetName, FMOD_SOUNDGROUP, 256);

#define FMODExt_func_FMOD_DSP_GetInfo _FMODExt_func_FMOD_DSP_GetInfo
static int _FMODExt_func_FMOD_DSP_GetInfo(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    char name[32];
    unsigned int version;
    int channels;
    int configwidth;
    int configheight;
    errCheck(FMOD_DSP_GetInfo(dsp, name, &version, &channels, &configwidth, &configheight));
    lua_pushstring(L, name);
    FMODExt_push_unsigned_int(L, version);
    FMODExt_push_int(L, channels);
    FMODExt_push_int(L, configwidth);
    FMODExt_push_int(L, configheight);
    return 5;
}

#define FMODExt_func_FMOD_Studio_System_LookupPath _FMODExt_func_FMOD_Studio_System_LookupPath
static int _FMODExt_func_FMOD_Studio_System_LookupPath(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const FMOD_GUID* id = FMODExt_check_ptr_FMOD_GUID(L, 2);
    int retrieved;
    errCheckBegin(FMOD_Studio_System_LookupPath(system, id, NULL, 0, &retrieved));
    errCheckEnd;
    char *path = (char*)malloc(retrieved);
    errCheckBegin(FMOD_Studio_System_LookupPath(system, id, path, retrieved, &retrieved)) {
        free(path);
    } errCheckEnd;
    lua_pushstring(L, path);
    free(path);
    return 1;
}

#define pathGetter(fname, type1) static int CONCAT(_FMODExt_func_, fname)(lua_State *L) { \
    type1* arg1 = CONCAT(FMODExt_check_ptr_, type1)(L, 1); \
    int retrieved; \
    errCheckBegin(fname(arg1, NULL, 0, &retrieved)); \
    errCheckEnd; \
    char *path = (char*)malloc(retrieved); \
    errCheckBegin(fname(arg1, path, retrieved, &retrieved)) { \
        free(path); \
    } errCheckEnd; \
    lua_pushstring(L, path); \
    free(path); \
    return 1; \
}

#define FMODExt_func_FMOD_Studio_EventDescription_GetPath _FMODExt_func_FMOD_Studio_EventDescription_GetPath
pathGetter(FMOD_Studio_EventDescription_GetPath, FMOD_STUDIO_EVENTDESCRIPTION)

#define FMODExt_func_FMOD_Studio_Bus_GetPath _FMODExt_func_FMOD_Studio_Bus_GetPath
pathGetter(FMOD_Studio_Bus_GetPath, FMOD_STUDIO_BUS)

#define FMODExt_func_FMOD_Studio_VCA_GetPath _FMODExt_func_FMOD_Studio_VCA_GetPath
pathGetter(FMOD_Studio_VCA_GetPath, FMOD_STUDIO_VCA)

#define FMODExt_func_FMOD_Studio_Bank_GetPath _FMODExt_func_FMOD_Studio_Bank_GetPath
pathGetter(FMOD_Studio_Bank_GetPath, FMOD_STUDIO_BANK)

#define FMODExt_func_FMOD_Studio_Bank_GetStringInfo _FMODExt_func_FMOD_Studio_Bank_GetStringInfo
static int _FMODExt_func_FMOD_Studio_Bank_GetStringInfo(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_GUID* id = FMODExt_push_ptr_FMOD_GUID(L, NULL);
    int retrieved;
    errCheckBegin(FMOD_Studio_Bank_GetStringInfo(bank, index, id, NULL, 0, &retrieved));
    errCheckEnd;
    char *path = (char*)malloc(retrieved);
    errCheckBegin(FMOD_Studio_Bank_GetStringInfo(bank, index, id, path, retrieved, &retrieved)) {
        free(path);
    } errCheckEnd;
    lua_pushstring(L, path);
    free(path);
    return 2;
}

#define FMODExt_func_FMOD_Studio_CommandReplay_GetCommandString _FMODExt_func_FMOD_Studio_CommandReplay_GetCommandString
static int _FMODExt_func_FMOD_Studio_CommandReplay_GetCommandString(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    int commandindex = FMODExt_check_int(L, 2);
    char buffer[512];
    errCheck(FMOD_Studio_CommandReplay_GetCommandString(replay, commandindex, buffer, 512));
    lua_pushstring(L, buffer);
    return 1;
}

/* Generated function definitions */

{% for f in functions %}
/* {{ f.name }}({% for arg in f.args %}{{ arg.usage }} {{ arg.type.c_type }} {{ arg.name }}, {% endfor %}) */
{% if f.generated %}#ifndef FMODExt_func_{{ f.name }}
#define FMODExt_func_{{ f.name }} _FMODExt_func_{{ f.name }}
static int _FMODExt_func_{{ f.name }}(lua_State *L) {
    {% for arg in f.args %}{% if arg.usage == "input" %}{{ arg.type.c_type }} {{ arg.name }} = {% if arg.optional %}optional(L, {{ arg.arg_index }}, {{ arg.type.c_type }}, {% endif %}FMODExt_check_{{ arg.type.name }}(L, {{ arg.arg_index }}){% if arg.optional %}){% endif %};
    {% elif arg.usage == "input_deref" %}{{ arg.type.c_type }}* {{ arg.name }} = {% if arg.optional %}optional(L, {{ arg.arg_index }}, {{ arg.type.c_type }}*, {% endif %}FMODExt_check_ptr_{{ arg.type.name }}(L, {{ arg.arg_index }}){% if arg.optional %}){% endif %};
    {% elif arg.usage == "input_ptr" %}{{ arg.type.child.c_type }} {{ arg.name }} = {% if arg.optional %}optional(L, {{ arg.arg_index }}, {{ arg.type.child.c_type }}, {% endif %}FMODExt_check_{{ arg.type.child.name }}(L, {{ arg.arg_index }}){% if arg.optional %}){% endif %};
    {% elif arg.usage == "output" %}{{ arg.type.child.c_type }} {{ arg.name }};
    {% elif arg.usage == "output_ptr" %}{{ arg.type.c_type }} {{ arg.name }} = FMODExt_push_{{ arg.type.name }}(L, NULL);
    {% endif %}{% endfor %}{% if f.returns_bool %}FMOD_BOOL retval = {{ f.name }}({% for arg in f.args %}{% if not loop.first %}, {% endif %}{{ arg.accessor }}{{ arg.name }}{% endfor %});
    lua_pushboolean(L, retval);
    return 1;{% else %}errCheck({{ f.name }}({% for arg in f.args %}{% if not loop.first %}, {% endif %}{{ arg.accessor }}{{ arg.name }}{% endfor %}));
    {% for arg in f.args %}{% if arg.usage == "output" %}FMODExt_push_{{ arg.type.child.name }}(L, {{ arg.name }});
    {% elif arg.usage == "output_ptr" %}lua_pushvalue(L, {{ arg.output_ptr_index - f.output_ptr_count - arg.output_index }});
    {% endif %}{% endfor %}{% if f.refcount_release %}
    lua_rawgeti(L, LUA_REGISTRYINDEX, FMODExt_registry_refcount);
    lua_pushlightuserdata(L, {{ f.args[0].name }});
    lua_pushnil(L);
    lua_rawset(L, -3);
    lua_pop(L, 1);
    {% endif %}return {{ f.return_count }};{% endif %}
}
#endif
{% endif %}
{% endfor %}

static int deprecatedWrapper(lua_State * L) {
    if (lua_toboolean(L, lua_upvalueindex(3))) {
        lua_pushboolean(L, 0);
        lua_replace(L, lua_upvalueindex(3));
        const char * message = lua_tostring(L, lua_upvalueindex(2));
        LOGW("%s", message);
    }

    int nargs = lua_gettop(L);
    lua_pushvalue(L, lua_upvalueindex(1));
    for (int i = 0; i < nargs; i += 1) {
        lua_pushvalue(L, i + 1);
    }
    lua_call(L, nargs, LUA_MULTRET);
    return lua_gettop(L) - nargs;
}

/* Register everything to the Lua namespace */

void FMODExt_register(lua_State *L) {
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setglobal(L, "fmod");

    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setfield(L, -3, "studio");

    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setfield(L, -4, "error_code");
    FMODExt_registry_FMOD_RESULT = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_newtable(L);
    FMODExt_registry_refcount = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_newtable(L);
    FMODExt_registry_rolloff = luaL_ref(L, LUA_REGISTRYINDEX);

    #define addEnum(x) \
        lua_pushstring(L, #x); \
        lua_pushnumber(L, (lua_Number)CONCAT(FMOD_, x)); \
        lua_rawset(L, -4)

    {% for enum in enums
        %}addEnum({{ enum }});
    {% endfor %}

    #define beginStruct(structName) \
        lua_newtable(L); \
        lua_pushvalue(L, -1); \
        CONCAT(FMODExt_registry_, structName) = luaL_ref(L, LUA_REGISTRYINDEX); \
        \
        lua_pushvalue(L, -1); \
        lua_pushcclosure(L, &structIndexMetamethod, 1); \
        lua_setfield(L, -2, "__index"); \
        \
        lua_pushvalue(L, -1); \
        lua_pushcclosure(L, &structNewIndexMetamethod, 1); \
        lua_setfield(L, -2, "__newindex"); \
        \
        lua_newtable(L); \
        lua_pushvalue(L, -1); \
        lua_setfield(L, -3, "__fieldget"); \
        \
        lua_newtable(L); \
        lua_pushvalue(L, -1); \
        lua_setfield(L, -4, "__fieldset")

    #define addStructEquality(structName) \
        lua_pushnumber(L, sizeof(structName)); \
        lua_pushcclosure(L, &structEqualityMetamethod, 1); \
        lua_setfield(L, -4, "__eq")

    #define addClassEquality(structName) \
        lua_pushnumber(L, sizeof(structName*)); \
        lua_pushcclosure(L, &structEqualityMetamethod, 1); \
        lua_setfield(L, -4, "__eq")

    #define addDestructor(structName, releaseFname) \
        lua_pushcfunction(L, &CONCAT(FMODExt_func_, releaseFname)); \
        lua_pushcclosure(L, &classGC, 1); \
        lua_setfield(L, -4, "__gc")

    #define endStruct() lua_pop(L, 3)

    #define addStructConstructor(structName, name, containerIndex) \
        lua_pushnumber(L, CONCAT(FMODExt_registry_, structName)); \
        lua_pushnumber(L, sizeof(structName)); \
        lua_pushcclosure(L, &structConstructor, 2); \
        lua_setfield(L, containerIndex - 4, name)

    #define addPropertyGetter(structName, name, typename) \
        lua_pushlightuserdata(L, (void*)offsetof(structName, name)); \
        lua_pushcclosure(L, &CONCAT(FMODExt_propertyGet_, typename), 1); \
        lua_setfield(L, -3, STRINGIFY(name))

    #define addPropertySetter(structName, name, typename) \
        lua_pushlightuserdata(L, (void*)offsetof(structName, name)); \
        lua_pushcclosure(L, &CONCAT(FMODExt_propertySet_, typename), 1); \
        lua_setfield(L, -2, STRINGIFY(name))

    #define addProperty(structName, name, typename) \
        addPropertyGetter(structName, name, typename); \
        addPropertySetter(structName, name, typename)

    beginStruct(long_long);
    addStructEquality(long long);
    lua_pushnumber(L, FMODExt_registry_long_long);
    lua_pushcclosure(L, &longLongConstructor, 1);
    lua_setfield(L, -6, "s64");
    lua_pushcfunction(L, &longLongGetValue);
    lua_setfield(L, -3, "value");
    lua_pushcfunction(L, &longLongGetLow);
    lua_setfield(L, -3, "low");
    lua_pushcfunction(L, &longLongGetHigh);
    lua_setfield(L, -3, "high");
    lua_pushcfunction(L, &longLongToString);
    lua_setfield(L, -4, "__tostring");
    endStruct();

    beginStruct(unsigned_long_long);
    addStructEquality(unsigned long long);
    lua_pushnumber(L, FMODExt_registry_unsigned_long_long);
    lua_pushcclosure(L, &longLongConstructor, 1);
    lua_setfield(L, -6, "u64");
    lua_pushcfunction(L, &unsignedLongLongGetValue);
    lua_setfield(L, -3, "value");
    lua_pushcfunction(L, &unsignedLongLongGetLow);
    lua_setfield(L, -3, "low");
    lua_pushcfunction(L, &unsignedLongLongGetHigh);
    lua_setfield(L, -3, "high");
    lua_pushcfunction(L, &unsignedLongLongToString);
    lua_setfield(L, -4, "__tostring");
    endStruct();

    {% for struct in structs %}beginStruct({{ struct.name }});
        {% if struct.is_class %}addClassEquality({{ struct.name }});
        {% else %}addStructEquality({{ struct.name }});
        {% endif %}{% if not struct.is_class %}addStructConstructor({{ struct.name }}, "{{ struct.constructor_name }}", {{ struct.constructor_table }});
        {% endif %}{% if struct.ref_counted %}addDestructor({{ struct.name }}, {{ struct.release_name }});
        {% endif %}{% for property in struct.properties %}/* {{ property[1].c_type }} {{ property[0] }} */
        {% if property[1].readable %}addPropertyGetter({{ struct.name }}, {{ property[0] }}, {{ property[1].name }});
        {% endif %}{% if property[1].writeable %}addPropertySetter({{ struct.name }}, {{ property[0] }}, {{ property[1].name }});
        {% endif %}{% endfor %}{% for f in struct.methods %}#ifdef FMODExt_func_{{ f[1].name }}
        lua_pushcfunction(L, &FMODExt_func_{{ f[1].name }});
        lua_setfield(L, -4, "{{ f[0] }}");
        #endif
        {% endfor %}#ifdef FMODExt_extras_{{ struct.name }}
        FMODExt_extras_{{ struct.name }}
        #endif
    endStruct();
    {% endfor %}

    {% for f in global_functions %}
        #ifdef FMODExt_func_{{ f[2].name }}
        lua_pushcfunction(L, &FMODExt_func_{{ f[2].name }});
        lua_setfield(L, {{ f[0] }} - 1, "{{ f[1] }}");
        #endif
    {% endfor %}

    FMODExt_push_ptr_FMOD_STUDIO_SYSTEM(L, FMODExt_system);
    lua_setfield(L, -2, "system");

    FMODExt_push_ptr_FMOD_SYSTEM(L, FMODExt_lowLevelSystem);
    lua_setfield(L, -3, "system");

    lua_pushcfunction(L, &FMODExt_getBundleRoot);

    lua_getglobal(L, "sys");
    lua_getfield(L, -1, "get_application_path");
    int hasGetAppPath = !lua_isnoneornil(L, -1);
    lua_pop(L, 2);

    if (hasGetAppPath) {
        lua_pushstring(L, "fmod.get_bundle_root() is deprecated. Use sys.get_application_path() instead");
        lua_pushboolean(L, 1);
        lua_pushcclosure(L, &deprecatedWrapper, 3);
    }

    lua_setfield(L, -3, "get_bundle_root");

    lua_pop(L, 2);
}
