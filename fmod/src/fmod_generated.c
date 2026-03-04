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


#define FMODExt_push_FMOD_THREAD_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_THREAD_TYPE(L, index) ((FMOD_THREAD_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_RESULT(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_RESULT(L, index) ((FMOD_RESULT)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_CHANNELCONTROL_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_CHANNELCONTROL_TYPE(L, index) ((FMOD_CHANNELCONTROL_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_OUTPUTTYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_OUTPUTTYPE(L, index) ((FMOD_OUTPUTTYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DEBUG_MODE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DEBUG_MODE(L, index) ((FMOD_DEBUG_MODE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_SPEAKERMODE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_SPEAKERMODE(L, index) ((FMOD_SPEAKERMODE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_SPEAKER(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_SPEAKER(L, index) ((FMOD_SPEAKER)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_CHANNELORDER(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_CHANNELORDER(L, index) ((FMOD_CHANNELORDER)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_PLUGINTYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_PLUGINTYPE(L, index) ((FMOD_PLUGINTYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_SOUND_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_SOUND_TYPE(L, index) ((FMOD_SOUND_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_SOUND_FORMAT(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_SOUND_FORMAT(L, index) ((FMOD_SOUND_FORMAT)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_OPENSTATE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_OPENSTATE(L, index) ((FMOD_OPENSTATE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_SOUNDGROUP_BEHAVIOR(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_SOUNDGROUP_BEHAVIOR(L, index) ((FMOD_SOUNDGROUP_BEHAVIOR)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_CHANNELCONTROL_CALLBACK_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_CHANNELCONTROL_CALLBACK_TYPE(L, index) ((FMOD_CHANNELCONTROL_CALLBACK_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_CHANNELCONTROL_DSP_INDEX(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_CHANNELCONTROL_DSP_INDEX(L, index) ((FMOD_CHANNELCONTROL_DSP_INDEX)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_ERRORCALLBACK_INSTANCETYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_ERRORCALLBACK_INSTANCETYPE(L, index) ((FMOD_ERRORCALLBACK_INSTANCETYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_RESAMPLER(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_RESAMPLER(L, index) ((FMOD_DSP_RESAMPLER)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_CALLBACK_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_CALLBACK_TYPE(L, index) ((FMOD_DSP_CALLBACK_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSPCONNECTION_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSPCONNECTION_TYPE(L, index) ((FMOD_DSPCONNECTION_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_TAGTYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_TAGTYPE(L, index) ((FMOD_TAGTYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_TAGDATATYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_TAGDATATYPE(L, index) ((FMOD_TAGDATATYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_PORT_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_PORT_TYPE(L, index) ((FMOD_PORT_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_TYPE(L, index) ((FMOD_DSP_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_OSCILLATOR(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_OSCILLATOR(L, index) ((FMOD_DSP_OSCILLATOR)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_LOWPASS(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_LOWPASS(L, index) ((FMOD_DSP_LOWPASS)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_ITLOWPASS(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_ITLOWPASS(L, index) ((FMOD_DSP_ITLOWPASS)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_HIGHPASS(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_HIGHPASS(L, index) ((FMOD_DSP_HIGHPASS)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_ECHO(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_ECHO(L, index) ((FMOD_DSP_ECHO)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_ECHO_DELAYCHANGEMODE_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_ECHO_DELAYCHANGEMODE_TYPE(L, index) ((FMOD_DSP_ECHO_DELAYCHANGEMODE_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_FADER(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_FADER(L, index) ((FMOD_DSP_FADER)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_FLANGE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_FLANGE(L, index) ((FMOD_DSP_FLANGE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_DISTORTION(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_DISTORTION(L, index) ((FMOD_DSP_DISTORTION)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_NORMALIZE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_NORMALIZE(L, index) ((FMOD_DSP_NORMALIZE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_LIMITER(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_LIMITER(L, index) ((FMOD_DSP_LIMITER)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PARAMEQ(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PARAMEQ(L, index) ((FMOD_DSP_PARAMEQ)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_MULTIBAND_EQ(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_MULTIBAND_EQ(L, index) ((FMOD_DSP_MULTIBAND_EQ)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_MULTIBAND_EQ_FILTER_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_MULTIBAND_EQ_FILTER_TYPE(L, index) ((FMOD_DSP_MULTIBAND_EQ_FILTER_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_MULTIBAND_DYNAMICS(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_MULTIBAND_DYNAMICS(L, index) ((FMOD_DSP_MULTIBAND_DYNAMICS)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_MULTIBAND_DYNAMICS_MODE_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_MULTIBAND_DYNAMICS_MODE_TYPE(L, index) ((FMOD_DSP_MULTIBAND_DYNAMICS_MODE_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PITCHSHIFT(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PITCHSHIFT(L, index) ((FMOD_DSP_PITCHSHIFT)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_CHORUS(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_CHORUS(L, index) ((FMOD_DSP_CHORUS)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_ITECHO(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_ITECHO(L, index) ((FMOD_DSP_ITECHO)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_COMPRESSOR(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_COMPRESSOR(L, index) ((FMOD_DSP_COMPRESSOR)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_SFXREVERB(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_SFXREVERB(L, index) ((FMOD_DSP_SFXREVERB)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_LOWPASS_SIMPLE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_LOWPASS_SIMPLE(L, index) ((FMOD_DSP_LOWPASS_SIMPLE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_DELAY(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_DELAY(L, index) ((FMOD_DSP_DELAY)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_TREMOLO(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_TREMOLO(L, index) ((FMOD_DSP_TREMOLO)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_SEND(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_SEND(L, index) ((FMOD_DSP_SEND)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_RETURN(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_RETURN(L, index) ((FMOD_DSP_RETURN)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_HIGHPASS_SIMPLE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_HIGHPASS_SIMPLE(L, index) ((FMOD_DSP_HIGHPASS_SIMPLE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PAN_2D_STEREO_MODE_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PAN_2D_STEREO_MODE_TYPE(L, index) ((FMOD_DSP_PAN_2D_STEREO_MODE_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PAN_MODE_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PAN_MODE_TYPE(L, index) ((FMOD_DSP_PAN_MODE_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PAN_3D_ROLLOFF_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PAN_3D_ROLLOFF_TYPE(L, index) ((FMOD_DSP_PAN_3D_ROLLOFF_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PAN_3D_EXTENT_MODE_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PAN_3D_EXTENT_MODE_TYPE(L, index) ((FMOD_DSP_PAN_3D_EXTENT_MODE_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PAN(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PAN(L, index) ((FMOD_DSP_PAN)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_THREE_EQ_CROSSOVERSLOPE_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_THREE_EQ_CROSSOVERSLOPE_TYPE(L, index) ((FMOD_DSP_THREE_EQ_CROSSOVERSLOPE_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_THREE_EQ(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_THREE_EQ(L, index) ((FMOD_DSP_THREE_EQ)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_FFT_WINDOW_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_FFT_WINDOW_TYPE(L, index) ((FMOD_DSP_FFT_WINDOW_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_FFT_DOWNMIX_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_FFT_DOWNMIX_TYPE(L, index) ((FMOD_DSP_FFT_DOWNMIX_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_FFT(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_FFT(L, index) ((FMOD_DSP_FFT)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_LOUDNESS_METER(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_LOUDNESS_METER(L, index) ((FMOD_DSP_LOUDNESS_METER)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_LOUDNESS_METER_STATE_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_LOUDNESS_METER_STATE_TYPE(L, index) ((FMOD_DSP_LOUDNESS_METER_STATE_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_CONVOLUTION_REVERB(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_CONVOLUTION_REVERB(L, index) ((FMOD_DSP_CONVOLUTION_REVERB)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_CHANNELMIX_OUTPUT(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_CHANNELMIX_OUTPUT(L, index) ((FMOD_DSP_CHANNELMIX_OUTPUT)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_CHANNELMIX(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_CHANNELMIX(L, index) ((FMOD_DSP_CHANNELMIX)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_TRANSCEIVER_SPEAKERMODE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_TRANSCEIVER_SPEAKERMODE(L, index) ((FMOD_DSP_TRANSCEIVER_SPEAKERMODE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_TRANSCEIVER(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_TRANSCEIVER(L, index) ((FMOD_DSP_TRANSCEIVER)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_OBJECTPAN(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_OBJECTPAN(L, index) ((FMOD_DSP_OBJECTPAN)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PROCESS_OPERATION(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PROCESS_OPERATION(L, index) ((FMOD_DSP_PROCESS_OPERATION)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PAN_SURROUND_FLAGS(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PAN_SURROUND_FLAGS(L, index) ((FMOD_DSP_PAN_SURROUND_FLAGS)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PARAMETER_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PARAMETER_TYPE(L, index) ((FMOD_DSP_PARAMETER_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE(L, index) ((FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_DSP_PARAMETER_DATA_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_DSP_PARAMETER_DATA_TYPE(L, index) ((FMOD_DSP_PARAMETER_DATA_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_STUDIO_LOADING_STATE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_STUDIO_LOADING_STATE(L, index) ((FMOD_STUDIO_LOADING_STATE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_STUDIO_LOAD_MEMORY_MODE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_STUDIO_LOAD_MEMORY_MODE(L, index) ((FMOD_STUDIO_LOAD_MEMORY_MODE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_STUDIO_PARAMETER_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_STUDIO_PARAMETER_TYPE(L, index) ((FMOD_STUDIO_PARAMETER_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_STUDIO_USER_PROPERTY_TYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_STUDIO_USER_PROPERTY_TYPE(L, index) ((FMOD_STUDIO_USER_PROPERTY_TYPE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_STUDIO_EVENT_PROPERTY(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_STUDIO_EVENT_PROPERTY(L, index) ((FMOD_STUDIO_EVENT_PROPERTY)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_STUDIO_PLAYBACK_STATE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_STUDIO_PLAYBACK_STATE(L, index) ((FMOD_STUDIO_PLAYBACK_STATE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_STUDIO_STOP_MODE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_STUDIO_STOP_MODE(L, index) ((FMOD_STUDIO_STOP_MODE)luaL_checknumber(L, index))

#define FMODExt_push_FMOD_STUDIO_INSTANCETYPE(L, x) lua_pushnumber(L, (lua_Number)(x))
#define FMODExt_check_FMOD_STUDIO_INSTANCETYPE(L, index) ((FMOD_STUDIO_INSTANCETYPE)luaL_checknumber(L, index))


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

static int FMODExt_registry_FMOD_SYSTEM = LUA_REFNIL;
static int FMODExt_registry_FMOD_SOUND = LUA_REFNIL;
static int FMODExt_registry_FMOD_CHANNELCONTROL = LUA_REFNIL;
static int FMODExt_registry_FMOD_CHANNEL = LUA_REFNIL;
static int FMODExt_registry_FMOD_CHANNELGROUP = LUA_REFNIL;
static int FMODExt_registry_FMOD_SOUNDGROUP = LUA_REFNIL;
static int FMODExt_registry_FMOD_REVERB3D = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSPCONNECTION = LUA_REFNIL;
static int FMODExt_registry_FMOD_POLYGON = LUA_REFNIL;
static int FMODExt_registry_FMOD_GEOMETRY = LUA_REFNIL;
static int FMODExt_registry_FMOD_SYNCPOINT = LUA_REFNIL;
static int FMODExt_registry_FMOD_ASYNCREADINFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_3D_ATTRIBUTES = LUA_REFNIL;
static int FMODExt_registry_FMOD_GUID = LUA_REFNIL;
static int FMODExt_registry_FMOD_PLUGINLIST = LUA_REFNIL;
static int FMODExt_registry_FMOD_ADVANCEDSETTINGS = LUA_REFNIL;
static int FMODExt_registry_FMOD_TAG = LUA_REFNIL;
static int FMODExt_registry_FMOD_CREATESOUNDEXINFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_REVERB_PROPERTIES = LUA_REFNIL;
static int FMODExt_registry_FMOD_ERRORCALLBACK_INFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_CPU_USAGE = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_DATA_PARAMETER_INFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_CODEC_STATE = LUA_REFNIL;
static int FMODExt_registry_FMOD_CODEC_WAVEFORMAT = LUA_REFNIL;
static int FMODExt_registry_FMOD_CODEC_DESCRIPTION = LUA_REFNIL;
static int FMODExt_registry_FMOD_CODEC_STATE_FUNCTIONS = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_LOUDNESS_METER_INFO_TYPE = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_STATE = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_BUFFER_ARRAY = LUA_REFNIL;
static int FMODExt_registry_FMOD_COMPLEX = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_FLOAT_MAPPING = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_DESC_FLOAT = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_DESC_INT = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_DESC_BOOL = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_DESC_DATA = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_DESC = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_OVERALLGAIN = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_3DATTRIBUTES = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_ATTENUATION_RANGE = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_SIDECHAIN = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_FFT = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_DESCRIPTION = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_STATE_DFT_FUNCTIONS = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_STATE_PAN_FUNCTIONS = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_STATE_FUNCTIONS = LUA_REFNIL;
static int FMODExt_registry_FMOD_DSP_METERING_INFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_OUTPUT_STATE = LUA_REFNIL;
static int FMODExt_registry_FMOD_OUTPUT_OBJECT3DINFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_OUTPUT_DESCRIPTION = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_SYSTEM = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_EVENTDESCRIPTION = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_EVENTINSTANCE = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_BUS = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_VCA = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_BANK = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_COMMANDREPLAY = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_BANK_INFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_PARAMETER_ID = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_PARAMETER_DESCRIPTION = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_USER_PROPERTY = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_ADVANCEDSETTINGS = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_CPU_USAGE = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_BUFFER_INFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_BUFFER_USAGE = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_SOUND_INFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_COMMAND_INFO = LUA_REFNIL;
static int FMODExt_registry_FMOD_STUDIO_MEMORY_USAGE = LUA_REFNIL;


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


#define FMODExt_propertyGet_FMOD_THREAD_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_THREAD_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_RESULT FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_RESULT FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_CHANNELCONTROL_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_CHANNELCONTROL_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_OUTPUTTYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_OUTPUTTYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DEBUG_MODE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DEBUG_MODE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_SPEAKERMODE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_SPEAKERMODE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_SPEAKER FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_SPEAKER FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_CHANNELORDER FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_CHANNELORDER FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_PLUGINTYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_PLUGINTYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_SOUND_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_SOUND_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_SOUND_FORMAT FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_SOUND_FORMAT FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_OPENSTATE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_OPENSTATE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_SOUNDGROUP_BEHAVIOR FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_SOUNDGROUP_BEHAVIOR FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_CHANNELCONTROL_CALLBACK_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_CHANNELCONTROL_CALLBACK_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_CHANNELCONTROL_DSP_INDEX FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_CHANNELCONTROL_DSP_INDEX FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_ERRORCALLBACK_INSTANCETYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_ERRORCALLBACK_INSTANCETYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_RESAMPLER FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_RESAMPLER FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_CALLBACK_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_CALLBACK_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSPCONNECTION_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSPCONNECTION_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_TAGTYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_TAGTYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_TAGDATATYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_TAGDATATYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_PORT_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_PORT_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_OSCILLATOR FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_OSCILLATOR FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_LOWPASS FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_LOWPASS FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_ITLOWPASS FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_ITLOWPASS FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_HIGHPASS FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_HIGHPASS FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_ECHO FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_ECHO FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_ECHO_DELAYCHANGEMODE_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_ECHO_DELAYCHANGEMODE_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_FADER FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_FADER FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_FLANGE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_FLANGE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_DISTORTION FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_DISTORTION FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_NORMALIZE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_NORMALIZE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_LIMITER FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_LIMITER FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PARAMEQ FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PARAMEQ FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_MULTIBAND_EQ FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_MULTIBAND_EQ FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_MULTIBAND_EQ_FILTER_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_MULTIBAND_EQ_FILTER_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_MULTIBAND_DYNAMICS FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_MULTIBAND_DYNAMICS FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_MULTIBAND_DYNAMICS_MODE_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_MULTIBAND_DYNAMICS_MODE_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PITCHSHIFT FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PITCHSHIFT FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_CHORUS FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_CHORUS FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_ITECHO FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_ITECHO FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_COMPRESSOR FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_COMPRESSOR FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_SFXREVERB FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_SFXREVERB FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_LOWPASS_SIMPLE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_LOWPASS_SIMPLE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_DELAY FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_DELAY FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_TREMOLO FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_TREMOLO FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_SEND FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_SEND FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_RETURN FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_RETURN FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_HIGHPASS_SIMPLE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_HIGHPASS_SIMPLE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PAN_2D_STEREO_MODE_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PAN_2D_STEREO_MODE_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PAN_MODE_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PAN_MODE_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PAN_3D_ROLLOFF_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PAN_3D_ROLLOFF_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PAN_3D_EXTENT_MODE_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PAN_3D_EXTENT_MODE_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PAN FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PAN FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_THREE_EQ_CROSSOVERSLOPE_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_THREE_EQ_CROSSOVERSLOPE_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_THREE_EQ FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_THREE_EQ FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_FFT_WINDOW_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_FFT_WINDOW_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_FFT_DOWNMIX_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_FFT_DOWNMIX_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_FFT FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_FFT FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_LOUDNESS_METER FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_LOUDNESS_METER FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_LOUDNESS_METER_STATE_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_LOUDNESS_METER_STATE_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_CONVOLUTION_REVERB FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_CONVOLUTION_REVERB FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_CHANNELMIX_OUTPUT FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_CHANNELMIX_OUTPUT FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_CHANNELMIX FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_CHANNELMIX FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_TRANSCEIVER_SPEAKERMODE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_TRANSCEIVER_SPEAKERMODE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_TRANSCEIVER FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_TRANSCEIVER FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_OBJECTPAN FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_OBJECTPAN FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PROCESS_OPERATION FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PROCESS_OPERATION FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PAN_SURROUND_FLAGS FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PAN_SURROUND_FLAGS FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PARAMETER_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PARAMETER_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_DSP_PARAMETER_DATA_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_DSP_PARAMETER_DATA_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_STUDIO_LOADING_STATE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_STUDIO_LOADING_STATE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_STUDIO_LOAD_MEMORY_MODE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_STUDIO_LOAD_MEMORY_MODE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_STUDIO_PARAMETER_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_STUDIO_PARAMETER_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_STUDIO_USER_PROPERTY_TYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_STUDIO_USER_PROPERTY_TYPE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_STUDIO_EVENT_PROPERTY FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_STUDIO_EVENT_PROPERTY FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_STUDIO_PLAYBACK_STATE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_STUDIO_PLAYBACK_STATE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_STUDIO_STOP_MODE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_STUDIO_STOP_MODE FMODExt_propertySet_int

#define FMODExt_propertyGet_FMOD_STUDIO_INSTANCETYPE FMODExt_propertyGet_int
#define FMODExt_propertySet_FMOD_STUDIO_INSTANCETYPE FMODExt_propertySet_int



#ifndef FMODExt_push_ptr_FMOD_SYSTEM
#define FMODExt_push_ptr_FMOD_SYSTEM(L, instance) ((FMOD_SYSTEM*)pushClass(L, instance, FMODExt_registry_FMOD_SYSTEM))
#endif
#ifndef FMODExt_check_ptr_FMOD_SYSTEM
#define FMODExt_check_ptr_FMOD_SYSTEM(L, index) ((FMOD_SYSTEM*)checkClass(L, index, FMODExt_registry_FMOD_SYSTEM, "FMOD_SYSTEM"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_SYSTEM
FMODExt_propertyOverride_FMOD_SYSTEM
#else
declarePropertyGetter(ptr_FMOD_SYSTEM, FMOD_SYSTEM*);
#endif

#ifndef FMODExt_push_ptr_FMOD_SOUND
#define FMODExt_push_ptr_FMOD_SOUND(L, instance) ((FMOD_SOUND*)pushClass(L, instance, FMODExt_registry_FMOD_SOUND))
#endif
#ifndef FMODExt_check_ptr_FMOD_SOUND
#define FMODExt_check_ptr_FMOD_SOUND(L, index) ((FMOD_SOUND*)checkClass(L, index, FMODExt_registry_FMOD_SOUND, "FMOD_SOUND"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_SOUND
FMODExt_propertyOverride_FMOD_SOUND
#else
declarePropertyGetter(ptr_FMOD_SOUND, FMOD_SOUND*);
#endif

#ifndef FMODExt_push_ptr_FMOD_CHANNELCONTROL
#define FMODExt_push_ptr_FMOD_CHANNELCONTROL(L, instance) ((FMOD_CHANNELCONTROL*)pushClass(L, instance, FMODExt_registry_FMOD_CHANNELCONTROL))
#endif
#ifndef FMODExt_check_ptr_FMOD_CHANNELCONTROL
#define FMODExt_check_ptr_FMOD_CHANNELCONTROL(L, index) ((FMOD_CHANNELCONTROL*)checkClass(L, index, FMODExt_registry_FMOD_CHANNELCONTROL, "FMOD_CHANNELCONTROL"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_CHANNELCONTROL
FMODExt_propertyOverride_FMOD_CHANNELCONTROL
#else
declarePropertyGetter(ptr_FMOD_CHANNELCONTROL, FMOD_CHANNELCONTROL*);
#endif

#ifndef FMODExt_push_ptr_FMOD_CHANNEL
#define FMODExt_push_ptr_FMOD_CHANNEL(L, instance) ((FMOD_CHANNEL*)pushClass(L, instance, FMODExt_registry_FMOD_CHANNEL))
#endif
#ifndef FMODExt_check_ptr_FMOD_CHANNEL
#define FMODExt_check_ptr_FMOD_CHANNEL(L, index) ((FMOD_CHANNEL*)checkClass(L, index, FMODExt_registry_FMOD_CHANNEL, "FMOD_CHANNEL"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_CHANNEL
FMODExt_propertyOverride_FMOD_CHANNEL
#else
declarePropertyGetter(ptr_FMOD_CHANNEL, FMOD_CHANNEL*);
#endif

#ifndef FMODExt_push_ptr_FMOD_CHANNELGROUP
#define FMODExt_push_ptr_FMOD_CHANNELGROUP(L, instance) ((FMOD_CHANNELGROUP*)pushClass(L, instance, FMODExt_registry_FMOD_CHANNELGROUP))
#endif
#ifndef FMODExt_check_ptr_FMOD_CHANNELGROUP
#define FMODExt_check_ptr_FMOD_CHANNELGROUP(L, index) ((FMOD_CHANNELGROUP*)checkClass(L, index, FMODExt_registry_FMOD_CHANNELGROUP, "FMOD_CHANNELGROUP"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_CHANNELGROUP
FMODExt_propertyOverride_FMOD_CHANNELGROUP
#else
declarePropertyGetter(ptr_FMOD_CHANNELGROUP, FMOD_CHANNELGROUP*);
#endif

#ifndef FMODExt_push_ptr_FMOD_SOUNDGROUP
#define FMODExt_push_ptr_FMOD_SOUNDGROUP(L, instance) ((FMOD_SOUNDGROUP*)pushClass(L, instance, FMODExt_registry_FMOD_SOUNDGROUP))
#endif
#ifndef FMODExt_check_ptr_FMOD_SOUNDGROUP
#define FMODExt_check_ptr_FMOD_SOUNDGROUP(L, index) ((FMOD_SOUNDGROUP*)checkClass(L, index, FMODExt_registry_FMOD_SOUNDGROUP, "FMOD_SOUNDGROUP"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_SOUNDGROUP
FMODExt_propertyOverride_FMOD_SOUNDGROUP
#else
declarePropertyGetter(ptr_FMOD_SOUNDGROUP, FMOD_SOUNDGROUP*);
#endif

#ifndef FMODExt_push_ptr_FMOD_REVERB3D
#define FMODExt_push_ptr_FMOD_REVERB3D(L, instance) ((FMOD_REVERB3D*)pushClass(L, instance, FMODExt_registry_FMOD_REVERB3D))
#endif
#ifndef FMODExt_check_ptr_FMOD_REVERB3D
#define FMODExt_check_ptr_FMOD_REVERB3D(L, index) ((FMOD_REVERB3D*)checkClass(L, index, FMODExt_registry_FMOD_REVERB3D, "FMOD_REVERB3D"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_REVERB3D
FMODExt_propertyOverride_FMOD_REVERB3D
#else
declarePropertyGetter(ptr_FMOD_REVERB3D, FMOD_REVERB3D*);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP
#define FMODExt_push_ptr_FMOD_DSP(L, instance) ((FMOD_DSP*)pushClass(L, instance, FMODExt_registry_FMOD_DSP))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP
#define FMODExt_check_ptr_FMOD_DSP(L, index) ((FMOD_DSP*)checkClass(L, index, FMODExt_registry_FMOD_DSP, "FMOD_DSP"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP
FMODExt_propertyOverride_FMOD_DSP
#else
declarePropertyGetter(ptr_FMOD_DSP, FMOD_DSP*);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSPCONNECTION
#define FMODExt_push_ptr_FMOD_DSPCONNECTION(L, instance) ((FMOD_DSPCONNECTION*)pushClass(L, instance, FMODExt_registry_FMOD_DSPCONNECTION))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSPCONNECTION
#define FMODExt_check_ptr_FMOD_DSPCONNECTION(L, index) ((FMOD_DSPCONNECTION*)checkClass(L, index, FMODExt_registry_FMOD_DSPCONNECTION, "FMOD_DSPCONNECTION"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSPCONNECTION
FMODExt_propertyOverride_FMOD_DSPCONNECTION
#else
declarePropertyGetter(ptr_FMOD_DSPCONNECTION, FMOD_DSPCONNECTION*);
#endif

#ifndef FMODExt_push_ptr_FMOD_POLYGON
#define FMODExt_push_ptr_FMOD_POLYGON(L, instance) ((FMOD_POLYGON*)pushClass(L, instance, FMODExt_registry_FMOD_POLYGON))
#endif
#ifndef FMODExt_check_ptr_FMOD_POLYGON
#define FMODExt_check_ptr_FMOD_POLYGON(L, index) ((FMOD_POLYGON*)checkClass(L, index, FMODExt_registry_FMOD_POLYGON, "FMOD_POLYGON"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_POLYGON
FMODExt_propertyOverride_FMOD_POLYGON
#else
declarePropertyGetter(ptr_FMOD_POLYGON, FMOD_POLYGON*);
#endif

#ifndef FMODExt_push_ptr_FMOD_GEOMETRY
#define FMODExt_push_ptr_FMOD_GEOMETRY(L, instance) ((FMOD_GEOMETRY*)pushClass(L, instance, FMODExt_registry_FMOD_GEOMETRY))
#endif
#ifndef FMODExt_check_ptr_FMOD_GEOMETRY
#define FMODExt_check_ptr_FMOD_GEOMETRY(L, index) ((FMOD_GEOMETRY*)checkClass(L, index, FMODExt_registry_FMOD_GEOMETRY, "FMOD_GEOMETRY"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_GEOMETRY
FMODExt_propertyOverride_FMOD_GEOMETRY
#else
declarePropertyGetter(ptr_FMOD_GEOMETRY, FMOD_GEOMETRY*);
#endif

#ifndef FMODExt_push_ptr_FMOD_SYNCPOINT
#define FMODExt_push_ptr_FMOD_SYNCPOINT(L, instance) ((FMOD_SYNCPOINT*)pushClass(L, instance, FMODExt_registry_FMOD_SYNCPOINT))
#endif
#ifndef FMODExt_check_ptr_FMOD_SYNCPOINT
#define FMODExt_check_ptr_FMOD_SYNCPOINT(L, index) ((FMOD_SYNCPOINT*)checkClass(L, index, FMODExt_registry_FMOD_SYNCPOINT, "FMOD_SYNCPOINT"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_SYNCPOINT
FMODExt_propertyOverride_FMOD_SYNCPOINT
#else
declarePropertyGetter(ptr_FMOD_SYNCPOINT, FMOD_SYNCPOINT*);
#endif

#ifndef FMODExt_push_ptr_FMOD_ASYNCREADINFO
#define FMODExt_push_ptr_FMOD_ASYNCREADINFO(L, structData) ((FMOD_ASYNCREADINFO*)pushStruct(L, structData, sizeof(FMOD_ASYNCREADINFO), FMODExt_registry_FMOD_ASYNCREADINFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_ASYNCREADINFO
#define FMODExt_check_ptr_FMOD_ASYNCREADINFO(L, index) ((FMOD_ASYNCREADINFO*)checkStruct(L, index, FMODExt_registry_FMOD_ASYNCREADINFO, "FMOD_ASYNCREADINFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_ASYNCREADINFO
FMODExt_propertyOverride_FMOD_ASYNCREADINFO
#else
declarePropertyGetter(ptr_FMOD_ASYNCREADINFO, FMOD_ASYNCREADINFO*);
declarePropertyGetterPtr(FMOD_ASYNCREADINFO, FMOD_ASYNCREADINFO);
declarePropertySetterPtr(FMOD_ASYNCREADINFO, FMOD_ASYNCREADINFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_3D_ATTRIBUTES
#define FMODExt_push_ptr_FMOD_3D_ATTRIBUTES(L, structData) ((FMOD_3D_ATTRIBUTES*)pushStruct(L, structData, sizeof(FMOD_3D_ATTRIBUTES), FMODExt_registry_FMOD_3D_ATTRIBUTES))
#endif
#ifndef FMODExt_check_ptr_FMOD_3D_ATTRIBUTES
#define FMODExt_check_ptr_FMOD_3D_ATTRIBUTES(L, index) ((FMOD_3D_ATTRIBUTES*)checkStruct(L, index, FMODExt_registry_FMOD_3D_ATTRIBUTES, "FMOD_3D_ATTRIBUTES"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_3D_ATTRIBUTES
FMODExt_propertyOverride_FMOD_3D_ATTRIBUTES
#else
declarePropertyGetter(ptr_FMOD_3D_ATTRIBUTES, FMOD_3D_ATTRIBUTES*);
declarePropertyGetterPtr(FMOD_3D_ATTRIBUTES, FMOD_3D_ATTRIBUTES);
declarePropertySetterPtr(FMOD_3D_ATTRIBUTES, FMOD_3D_ATTRIBUTES);
#endif

#ifndef FMODExt_push_ptr_FMOD_GUID
#define FMODExt_push_ptr_FMOD_GUID(L, structData) ((FMOD_GUID*)pushStruct(L, structData, sizeof(FMOD_GUID), FMODExt_registry_FMOD_GUID))
#endif
#ifndef FMODExt_check_ptr_FMOD_GUID
#define FMODExt_check_ptr_FMOD_GUID(L, index) ((FMOD_GUID*)checkStruct(L, index, FMODExt_registry_FMOD_GUID, "FMOD_GUID"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_GUID
FMODExt_propertyOverride_FMOD_GUID
#else
declarePropertyGetter(ptr_FMOD_GUID, FMOD_GUID*);
declarePropertyGetterPtr(FMOD_GUID, FMOD_GUID);
declarePropertySetterPtr(FMOD_GUID, FMOD_GUID);
#endif

#ifndef FMODExt_push_ptr_FMOD_PLUGINLIST
#define FMODExt_push_ptr_FMOD_PLUGINLIST(L, structData) ((FMOD_PLUGINLIST*)pushStruct(L, structData, sizeof(FMOD_PLUGINLIST), FMODExt_registry_FMOD_PLUGINLIST))
#endif
#ifndef FMODExt_check_ptr_FMOD_PLUGINLIST
#define FMODExt_check_ptr_FMOD_PLUGINLIST(L, index) ((FMOD_PLUGINLIST*)checkStruct(L, index, FMODExt_registry_FMOD_PLUGINLIST, "FMOD_PLUGINLIST"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_PLUGINLIST
FMODExt_propertyOverride_FMOD_PLUGINLIST
#else
declarePropertyGetter(ptr_FMOD_PLUGINLIST, FMOD_PLUGINLIST*);
declarePropertyGetterPtr(FMOD_PLUGINLIST, FMOD_PLUGINLIST);
declarePropertySetterPtr(FMOD_PLUGINLIST, FMOD_PLUGINLIST);
#endif

#ifndef FMODExt_push_ptr_FMOD_ADVANCEDSETTINGS
#define FMODExt_push_ptr_FMOD_ADVANCEDSETTINGS(L, structData) ((FMOD_ADVANCEDSETTINGS*)pushStruct(L, structData, sizeof(FMOD_ADVANCEDSETTINGS), FMODExt_registry_FMOD_ADVANCEDSETTINGS))
#endif
#ifndef FMODExt_check_ptr_FMOD_ADVANCEDSETTINGS
#define FMODExt_check_ptr_FMOD_ADVANCEDSETTINGS(L, index) ((FMOD_ADVANCEDSETTINGS*)checkStruct(L, index, FMODExt_registry_FMOD_ADVANCEDSETTINGS, "FMOD_ADVANCEDSETTINGS"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_ADVANCEDSETTINGS
FMODExt_propertyOverride_FMOD_ADVANCEDSETTINGS
#else
declarePropertyGetter(ptr_FMOD_ADVANCEDSETTINGS, FMOD_ADVANCEDSETTINGS*);
declarePropertyGetterPtr(FMOD_ADVANCEDSETTINGS, FMOD_ADVANCEDSETTINGS);
declarePropertySetterPtr(FMOD_ADVANCEDSETTINGS, FMOD_ADVANCEDSETTINGS);
#endif

#ifndef FMODExt_push_ptr_FMOD_TAG
#define FMODExt_push_ptr_FMOD_TAG(L, structData) ((FMOD_TAG*)pushStruct(L, structData, sizeof(FMOD_TAG), FMODExt_registry_FMOD_TAG))
#endif
#ifndef FMODExt_check_ptr_FMOD_TAG
#define FMODExt_check_ptr_FMOD_TAG(L, index) ((FMOD_TAG*)checkStruct(L, index, FMODExt_registry_FMOD_TAG, "FMOD_TAG"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_TAG
FMODExt_propertyOverride_FMOD_TAG
#else
declarePropertyGetter(ptr_FMOD_TAG, FMOD_TAG*);
declarePropertyGetterPtr(FMOD_TAG, FMOD_TAG);
declarePropertySetterPtr(FMOD_TAG, FMOD_TAG);
#endif

#ifndef FMODExt_push_ptr_FMOD_CREATESOUNDEXINFO
#define FMODExt_push_ptr_FMOD_CREATESOUNDEXINFO(L, structData) ((FMOD_CREATESOUNDEXINFO*)pushStruct(L, structData, sizeof(FMOD_CREATESOUNDEXINFO), FMODExt_registry_FMOD_CREATESOUNDEXINFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_CREATESOUNDEXINFO
#define FMODExt_check_ptr_FMOD_CREATESOUNDEXINFO(L, index) ((FMOD_CREATESOUNDEXINFO*)checkStruct(L, index, FMODExt_registry_FMOD_CREATESOUNDEXINFO, "FMOD_CREATESOUNDEXINFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_CREATESOUNDEXINFO
FMODExt_propertyOverride_FMOD_CREATESOUNDEXINFO
#else
declarePropertyGetter(ptr_FMOD_CREATESOUNDEXINFO, FMOD_CREATESOUNDEXINFO*);
declarePropertyGetterPtr(FMOD_CREATESOUNDEXINFO, FMOD_CREATESOUNDEXINFO);
declarePropertySetterPtr(FMOD_CREATESOUNDEXINFO, FMOD_CREATESOUNDEXINFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_REVERB_PROPERTIES
#define FMODExt_push_ptr_FMOD_REVERB_PROPERTIES(L, structData) ((FMOD_REVERB_PROPERTIES*)pushStruct(L, structData, sizeof(FMOD_REVERB_PROPERTIES), FMODExt_registry_FMOD_REVERB_PROPERTIES))
#endif
#ifndef FMODExt_check_ptr_FMOD_REVERB_PROPERTIES
#define FMODExt_check_ptr_FMOD_REVERB_PROPERTIES(L, index) ((FMOD_REVERB_PROPERTIES*)checkStruct(L, index, FMODExt_registry_FMOD_REVERB_PROPERTIES, "FMOD_REVERB_PROPERTIES"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_REVERB_PROPERTIES
FMODExt_propertyOverride_FMOD_REVERB_PROPERTIES
#else
declarePropertyGetter(ptr_FMOD_REVERB_PROPERTIES, FMOD_REVERB_PROPERTIES*);
declarePropertyGetterPtr(FMOD_REVERB_PROPERTIES, FMOD_REVERB_PROPERTIES);
declarePropertySetterPtr(FMOD_REVERB_PROPERTIES, FMOD_REVERB_PROPERTIES);
#endif

#ifndef FMODExt_push_ptr_FMOD_ERRORCALLBACK_INFO
#define FMODExt_push_ptr_FMOD_ERRORCALLBACK_INFO(L, structData) ((FMOD_ERRORCALLBACK_INFO*)pushStruct(L, structData, sizeof(FMOD_ERRORCALLBACK_INFO), FMODExt_registry_FMOD_ERRORCALLBACK_INFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_ERRORCALLBACK_INFO
#define FMODExt_check_ptr_FMOD_ERRORCALLBACK_INFO(L, index) ((FMOD_ERRORCALLBACK_INFO*)checkStruct(L, index, FMODExt_registry_FMOD_ERRORCALLBACK_INFO, "FMOD_ERRORCALLBACK_INFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_ERRORCALLBACK_INFO
FMODExt_propertyOverride_FMOD_ERRORCALLBACK_INFO
#else
declarePropertyGetter(ptr_FMOD_ERRORCALLBACK_INFO, FMOD_ERRORCALLBACK_INFO*);
declarePropertyGetterPtr(FMOD_ERRORCALLBACK_INFO, FMOD_ERRORCALLBACK_INFO);
declarePropertySetterPtr(FMOD_ERRORCALLBACK_INFO, FMOD_ERRORCALLBACK_INFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_CPU_USAGE
#define FMODExt_push_ptr_FMOD_CPU_USAGE(L, structData) ((FMOD_CPU_USAGE*)pushStruct(L, structData, sizeof(FMOD_CPU_USAGE), FMODExt_registry_FMOD_CPU_USAGE))
#endif
#ifndef FMODExt_check_ptr_FMOD_CPU_USAGE
#define FMODExt_check_ptr_FMOD_CPU_USAGE(L, index) ((FMOD_CPU_USAGE*)checkStruct(L, index, FMODExt_registry_FMOD_CPU_USAGE, "FMOD_CPU_USAGE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_CPU_USAGE
FMODExt_propertyOverride_FMOD_CPU_USAGE
#else
declarePropertyGetter(ptr_FMOD_CPU_USAGE, FMOD_CPU_USAGE*);
declarePropertyGetterPtr(FMOD_CPU_USAGE, FMOD_CPU_USAGE);
declarePropertySetterPtr(FMOD_CPU_USAGE, FMOD_CPU_USAGE);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_DATA_PARAMETER_INFO
#define FMODExt_push_ptr_FMOD_DSP_DATA_PARAMETER_INFO(L, structData) ((FMOD_DSP_DATA_PARAMETER_INFO*)pushStruct(L, structData, sizeof(FMOD_DSP_DATA_PARAMETER_INFO), FMODExt_registry_FMOD_DSP_DATA_PARAMETER_INFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_DATA_PARAMETER_INFO
#define FMODExt_check_ptr_FMOD_DSP_DATA_PARAMETER_INFO(L, index) ((FMOD_DSP_DATA_PARAMETER_INFO*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_DATA_PARAMETER_INFO, "FMOD_DSP_DATA_PARAMETER_INFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_DATA_PARAMETER_INFO
FMODExt_propertyOverride_FMOD_DSP_DATA_PARAMETER_INFO
#else
declarePropertyGetter(ptr_FMOD_DSP_DATA_PARAMETER_INFO, FMOD_DSP_DATA_PARAMETER_INFO*);
declarePropertyGetterPtr(FMOD_DSP_DATA_PARAMETER_INFO, FMOD_DSP_DATA_PARAMETER_INFO);
declarePropertySetterPtr(FMOD_DSP_DATA_PARAMETER_INFO, FMOD_DSP_DATA_PARAMETER_INFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_CODEC_STATE
#define FMODExt_push_ptr_FMOD_CODEC_STATE(L, structData) ((FMOD_CODEC_STATE*)pushStruct(L, structData, sizeof(FMOD_CODEC_STATE), FMODExt_registry_FMOD_CODEC_STATE))
#endif
#ifndef FMODExt_check_ptr_FMOD_CODEC_STATE
#define FMODExt_check_ptr_FMOD_CODEC_STATE(L, index) ((FMOD_CODEC_STATE*)checkStruct(L, index, FMODExt_registry_FMOD_CODEC_STATE, "FMOD_CODEC_STATE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_CODEC_STATE
FMODExt_propertyOverride_FMOD_CODEC_STATE
#else
declarePropertyGetter(ptr_FMOD_CODEC_STATE, FMOD_CODEC_STATE*);
declarePropertyGetterPtr(FMOD_CODEC_STATE, FMOD_CODEC_STATE);
declarePropertySetterPtr(FMOD_CODEC_STATE, FMOD_CODEC_STATE);
#endif

#ifndef FMODExt_push_ptr_FMOD_CODEC_WAVEFORMAT
#define FMODExt_push_ptr_FMOD_CODEC_WAVEFORMAT(L, structData) ((FMOD_CODEC_WAVEFORMAT*)pushStruct(L, structData, sizeof(FMOD_CODEC_WAVEFORMAT), FMODExt_registry_FMOD_CODEC_WAVEFORMAT))
#endif
#ifndef FMODExt_check_ptr_FMOD_CODEC_WAVEFORMAT
#define FMODExt_check_ptr_FMOD_CODEC_WAVEFORMAT(L, index) ((FMOD_CODEC_WAVEFORMAT*)checkStruct(L, index, FMODExt_registry_FMOD_CODEC_WAVEFORMAT, "FMOD_CODEC_WAVEFORMAT"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_CODEC_WAVEFORMAT
FMODExt_propertyOverride_FMOD_CODEC_WAVEFORMAT
#else
declarePropertyGetter(ptr_FMOD_CODEC_WAVEFORMAT, FMOD_CODEC_WAVEFORMAT*);
declarePropertyGetterPtr(FMOD_CODEC_WAVEFORMAT, FMOD_CODEC_WAVEFORMAT);
declarePropertySetterPtr(FMOD_CODEC_WAVEFORMAT, FMOD_CODEC_WAVEFORMAT);
#endif

#ifndef FMODExt_push_ptr_FMOD_CODEC_DESCRIPTION
#define FMODExt_push_ptr_FMOD_CODEC_DESCRIPTION(L, structData) ((FMOD_CODEC_DESCRIPTION*)pushStruct(L, structData, sizeof(FMOD_CODEC_DESCRIPTION), FMODExt_registry_FMOD_CODEC_DESCRIPTION))
#endif
#ifndef FMODExt_check_ptr_FMOD_CODEC_DESCRIPTION
#define FMODExt_check_ptr_FMOD_CODEC_DESCRIPTION(L, index) ((FMOD_CODEC_DESCRIPTION*)checkStruct(L, index, FMODExt_registry_FMOD_CODEC_DESCRIPTION, "FMOD_CODEC_DESCRIPTION"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_CODEC_DESCRIPTION
FMODExt_propertyOverride_FMOD_CODEC_DESCRIPTION
#else
declarePropertyGetter(ptr_FMOD_CODEC_DESCRIPTION, FMOD_CODEC_DESCRIPTION*);
declarePropertyGetterPtr(FMOD_CODEC_DESCRIPTION, FMOD_CODEC_DESCRIPTION);
declarePropertySetterPtr(FMOD_CODEC_DESCRIPTION, FMOD_CODEC_DESCRIPTION);
#endif

#ifndef FMODExt_push_ptr_FMOD_CODEC_STATE_FUNCTIONS
#define FMODExt_push_ptr_FMOD_CODEC_STATE_FUNCTIONS(L, structData) ((FMOD_CODEC_STATE_FUNCTIONS*)pushStruct(L, structData, sizeof(FMOD_CODEC_STATE_FUNCTIONS), FMODExt_registry_FMOD_CODEC_STATE_FUNCTIONS))
#endif
#ifndef FMODExt_check_ptr_FMOD_CODEC_STATE_FUNCTIONS
#define FMODExt_check_ptr_FMOD_CODEC_STATE_FUNCTIONS(L, index) ((FMOD_CODEC_STATE_FUNCTIONS*)checkStruct(L, index, FMODExt_registry_FMOD_CODEC_STATE_FUNCTIONS, "FMOD_CODEC_STATE_FUNCTIONS"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_CODEC_STATE_FUNCTIONS
FMODExt_propertyOverride_FMOD_CODEC_STATE_FUNCTIONS
#else
declarePropertyGetter(ptr_FMOD_CODEC_STATE_FUNCTIONS, FMOD_CODEC_STATE_FUNCTIONS*);
declarePropertyGetterPtr(FMOD_CODEC_STATE_FUNCTIONS, FMOD_CODEC_STATE_FUNCTIONS);
declarePropertySetterPtr(FMOD_CODEC_STATE_FUNCTIONS, FMOD_CODEC_STATE_FUNCTIONS);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_LOUDNESS_METER_INFO_TYPE
#define FMODExt_push_ptr_FMOD_DSP_LOUDNESS_METER_INFO_TYPE(L, structData) ((FMOD_DSP_LOUDNESS_METER_INFO_TYPE*)pushStruct(L, structData, sizeof(FMOD_DSP_LOUDNESS_METER_INFO_TYPE), FMODExt_registry_FMOD_DSP_LOUDNESS_METER_INFO_TYPE))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_LOUDNESS_METER_INFO_TYPE
#define FMODExt_check_ptr_FMOD_DSP_LOUDNESS_METER_INFO_TYPE(L, index) ((FMOD_DSP_LOUDNESS_METER_INFO_TYPE*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_LOUDNESS_METER_INFO_TYPE, "FMOD_DSP_LOUDNESS_METER_INFO_TYPE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_LOUDNESS_METER_INFO_TYPE
FMODExt_propertyOverride_FMOD_DSP_LOUDNESS_METER_INFO_TYPE
#else
declarePropertyGetter(ptr_FMOD_DSP_LOUDNESS_METER_INFO_TYPE, FMOD_DSP_LOUDNESS_METER_INFO_TYPE*);
declarePropertyGetterPtr(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, FMOD_DSP_LOUDNESS_METER_INFO_TYPE);
declarePropertySetterPtr(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, FMOD_DSP_LOUDNESS_METER_INFO_TYPE);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE
#define FMODExt_push_ptr_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE(L, structData) ((FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE*)pushStruct(L, structData, sizeof(FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE), FMODExt_registry_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE
#define FMODExt_check_ptr_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE(L, index) ((FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE, "FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE
FMODExt_propertyOverride_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE
#else
declarePropertyGetter(ptr_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE, FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE*);
declarePropertyGetterPtr(FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE, FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE);
declarePropertySetterPtr(FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE, FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_STATE
#define FMODExt_push_ptr_FMOD_DSP_STATE(L, structData) ((FMOD_DSP_STATE*)pushStruct(L, structData, sizeof(FMOD_DSP_STATE), FMODExt_registry_FMOD_DSP_STATE))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_STATE
#define FMODExt_check_ptr_FMOD_DSP_STATE(L, index) ((FMOD_DSP_STATE*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_STATE, "FMOD_DSP_STATE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_STATE
FMODExt_propertyOverride_FMOD_DSP_STATE
#else
declarePropertyGetter(ptr_FMOD_DSP_STATE, FMOD_DSP_STATE*);
declarePropertyGetterPtr(FMOD_DSP_STATE, FMOD_DSP_STATE);
declarePropertySetterPtr(FMOD_DSP_STATE, FMOD_DSP_STATE);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_BUFFER_ARRAY
#define FMODExt_push_ptr_FMOD_DSP_BUFFER_ARRAY(L, structData) ((FMOD_DSP_BUFFER_ARRAY*)pushStruct(L, structData, sizeof(FMOD_DSP_BUFFER_ARRAY), FMODExt_registry_FMOD_DSP_BUFFER_ARRAY))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_BUFFER_ARRAY
#define FMODExt_check_ptr_FMOD_DSP_BUFFER_ARRAY(L, index) ((FMOD_DSP_BUFFER_ARRAY*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_BUFFER_ARRAY, "FMOD_DSP_BUFFER_ARRAY"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_BUFFER_ARRAY
FMODExt_propertyOverride_FMOD_DSP_BUFFER_ARRAY
#else
declarePropertyGetter(ptr_FMOD_DSP_BUFFER_ARRAY, FMOD_DSP_BUFFER_ARRAY*);
declarePropertyGetterPtr(FMOD_DSP_BUFFER_ARRAY, FMOD_DSP_BUFFER_ARRAY);
declarePropertySetterPtr(FMOD_DSP_BUFFER_ARRAY, FMOD_DSP_BUFFER_ARRAY);
#endif

#ifndef FMODExt_push_ptr_FMOD_COMPLEX
#define FMODExt_push_ptr_FMOD_COMPLEX(L, structData) ((FMOD_COMPLEX*)pushStruct(L, structData, sizeof(FMOD_COMPLEX), FMODExt_registry_FMOD_COMPLEX))
#endif
#ifndef FMODExt_check_ptr_FMOD_COMPLEX
#define FMODExt_check_ptr_FMOD_COMPLEX(L, index) ((FMOD_COMPLEX*)checkStruct(L, index, FMODExt_registry_FMOD_COMPLEX, "FMOD_COMPLEX"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_COMPLEX
FMODExt_propertyOverride_FMOD_COMPLEX
#else
declarePropertyGetter(ptr_FMOD_COMPLEX, FMOD_COMPLEX*);
declarePropertyGetterPtr(FMOD_COMPLEX, FMOD_COMPLEX);
declarePropertySetterPtr(FMOD_COMPLEX, FMOD_COMPLEX);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR(L, structData) ((FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR), FMODExt_registry_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR(L, index) ((FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR, "FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR, FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR, FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR, FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING(L, structData) ((FMOD_DSP_PARAMETER_FLOAT_MAPPING*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_FLOAT_MAPPING), FMODExt_registry_FMOD_DSP_PARAMETER_FLOAT_MAPPING))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING(L, index) ((FMOD_DSP_PARAMETER_FLOAT_MAPPING*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_FLOAT_MAPPING, "FMOD_DSP_PARAMETER_FLOAT_MAPPING"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_FLOAT_MAPPING
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_FLOAT_MAPPING
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_FLOAT_MAPPING, FMOD_DSP_PARAMETER_FLOAT_MAPPING*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_FLOAT_MAPPING, FMOD_DSP_PARAMETER_FLOAT_MAPPING);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_FLOAT_MAPPING, FMOD_DSP_PARAMETER_FLOAT_MAPPING);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC_FLOAT
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC_FLOAT(L, structData) ((FMOD_DSP_PARAMETER_DESC_FLOAT*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_DESC_FLOAT), FMODExt_registry_FMOD_DSP_PARAMETER_DESC_FLOAT))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC_FLOAT
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC_FLOAT(L, index) ((FMOD_DSP_PARAMETER_DESC_FLOAT*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_DESC_FLOAT, "FMOD_DSP_PARAMETER_DESC_FLOAT"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC_FLOAT
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC_FLOAT
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_DESC_FLOAT, FMOD_DSP_PARAMETER_DESC_FLOAT*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_DESC_FLOAT, FMOD_DSP_PARAMETER_DESC_FLOAT);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_DESC_FLOAT, FMOD_DSP_PARAMETER_DESC_FLOAT);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC_INT
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC_INT(L, structData) ((FMOD_DSP_PARAMETER_DESC_INT*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_DESC_INT), FMODExt_registry_FMOD_DSP_PARAMETER_DESC_INT))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC_INT
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC_INT(L, index) ((FMOD_DSP_PARAMETER_DESC_INT*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_DESC_INT, "FMOD_DSP_PARAMETER_DESC_INT"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC_INT
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC_INT
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_DESC_INT, FMOD_DSP_PARAMETER_DESC_INT*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_DESC_INT, FMOD_DSP_PARAMETER_DESC_INT);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_DESC_INT, FMOD_DSP_PARAMETER_DESC_INT);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC_BOOL
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC_BOOL(L, structData) ((FMOD_DSP_PARAMETER_DESC_BOOL*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_DESC_BOOL), FMODExt_registry_FMOD_DSP_PARAMETER_DESC_BOOL))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC_BOOL
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC_BOOL(L, index) ((FMOD_DSP_PARAMETER_DESC_BOOL*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_DESC_BOOL, "FMOD_DSP_PARAMETER_DESC_BOOL"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC_BOOL
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC_BOOL
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_DESC_BOOL, FMOD_DSP_PARAMETER_DESC_BOOL*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_DESC_BOOL, FMOD_DSP_PARAMETER_DESC_BOOL);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_DESC_BOOL, FMOD_DSP_PARAMETER_DESC_BOOL);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC_DATA
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC_DATA(L, structData) ((FMOD_DSP_PARAMETER_DESC_DATA*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_DESC_DATA), FMODExt_registry_FMOD_DSP_PARAMETER_DESC_DATA))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC_DATA
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC_DATA(L, index) ((FMOD_DSP_PARAMETER_DESC_DATA*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_DESC_DATA, "FMOD_DSP_PARAMETER_DESC_DATA"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC_DATA
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC_DATA
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_DESC_DATA, FMOD_DSP_PARAMETER_DESC_DATA*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_DESC_DATA, FMOD_DSP_PARAMETER_DESC_DATA);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_DESC_DATA, FMOD_DSP_PARAMETER_DESC_DATA);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC(L, structData) ((FMOD_DSP_PARAMETER_DESC*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_DESC), FMODExt_registry_FMOD_DSP_PARAMETER_DESC))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_DESC(L, index) ((FMOD_DSP_PARAMETER_DESC*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_DESC, "FMOD_DSP_PARAMETER_DESC"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DESC
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_DESC, FMOD_DSP_PARAMETER_DESC*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_DESC, FMOD_DSP_PARAMETER_DESC);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_DESC, FMOD_DSP_PARAMETER_DESC);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_OVERALLGAIN
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_OVERALLGAIN(L, structData) ((FMOD_DSP_PARAMETER_OVERALLGAIN*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_OVERALLGAIN), FMODExt_registry_FMOD_DSP_PARAMETER_OVERALLGAIN))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_OVERALLGAIN
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_OVERALLGAIN(L, index) ((FMOD_DSP_PARAMETER_OVERALLGAIN*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_OVERALLGAIN, "FMOD_DSP_PARAMETER_OVERALLGAIN"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_OVERALLGAIN
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_OVERALLGAIN
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_OVERALLGAIN, FMOD_DSP_PARAMETER_OVERALLGAIN*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_OVERALLGAIN, FMOD_DSP_PARAMETER_OVERALLGAIN);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_OVERALLGAIN, FMOD_DSP_PARAMETER_OVERALLGAIN);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES(L, structData) ((FMOD_DSP_PARAMETER_3DATTRIBUTES*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_3DATTRIBUTES), FMODExt_registry_FMOD_DSP_PARAMETER_3DATTRIBUTES))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES(L, index) ((FMOD_DSP_PARAMETER_3DATTRIBUTES*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_3DATTRIBUTES, "FMOD_DSP_PARAMETER_3DATTRIBUTES"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_3DATTRIBUTES
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_3DATTRIBUTES
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES, FMOD_DSP_PARAMETER_3DATTRIBUTES*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_3DATTRIBUTES, FMOD_DSP_PARAMETER_3DATTRIBUTES);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_3DATTRIBUTES, FMOD_DSP_PARAMETER_3DATTRIBUTES);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI(L, structData) ((FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI), FMODExt_registry_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI(L, index) ((FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI, "FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI, FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI, FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI, FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_ATTENUATION_RANGE
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_ATTENUATION_RANGE(L, structData) ((FMOD_DSP_PARAMETER_ATTENUATION_RANGE*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_ATTENUATION_RANGE), FMODExt_registry_FMOD_DSP_PARAMETER_ATTENUATION_RANGE))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_ATTENUATION_RANGE
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_ATTENUATION_RANGE(L, index) ((FMOD_DSP_PARAMETER_ATTENUATION_RANGE*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_ATTENUATION_RANGE, "FMOD_DSP_PARAMETER_ATTENUATION_RANGE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_ATTENUATION_RANGE
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_ATTENUATION_RANGE
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_ATTENUATION_RANGE, FMOD_DSP_PARAMETER_ATTENUATION_RANGE*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_ATTENUATION_RANGE, FMOD_DSP_PARAMETER_ATTENUATION_RANGE);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_ATTENUATION_RANGE, FMOD_DSP_PARAMETER_ATTENUATION_RANGE);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_SIDECHAIN
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_SIDECHAIN(L, structData) ((FMOD_DSP_PARAMETER_SIDECHAIN*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_SIDECHAIN), FMODExt_registry_FMOD_DSP_PARAMETER_SIDECHAIN))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_SIDECHAIN
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_SIDECHAIN(L, index) ((FMOD_DSP_PARAMETER_SIDECHAIN*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_SIDECHAIN, "FMOD_DSP_PARAMETER_SIDECHAIN"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_SIDECHAIN
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_SIDECHAIN
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_SIDECHAIN, FMOD_DSP_PARAMETER_SIDECHAIN*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_SIDECHAIN, FMOD_DSP_PARAMETER_SIDECHAIN);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_SIDECHAIN, FMOD_DSP_PARAMETER_SIDECHAIN);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_FFT
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_FFT(L, structData) ((FMOD_DSP_PARAMETER_FFT*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_FFT), FMODExt_registry_FMOD_DSP_PARAMETER_FFT))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_FFT
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_FFT(L, index) ((FMOD_DSP_PARAMETER_FFT*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_FFT, "FMOD_DSP_PARAMETER_FFT"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_FFT
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_FFT
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_FFT, FMOD_DSP_PARAMETER_FFT*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_FFT, FMOD_DSP_PARAMETER_FFT);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_FFT, FMOD_DSP_PARAMETER_FFT);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE
#define FMODExt_push_ptr_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE(L, structData) ((FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE*)pushStruct(L, structData, sizeof(FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE), FMODExt_registry_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE
#define FMODExt_check_ptr_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE(L, index) ((FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE, "FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE
FMODExt_propertyOverride_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE
#else
declarePropertyGetter(ptr_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE, FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE*);
declarePropertyGetterPtr(FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE, FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE);
declarePropertySetterPtr(FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE, FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_DESCRIPTION
#define FMODExt_push_ptr_FMOD_DSP_DESCRIPTION(L, structData) ((FMOD_DSP_DESCRIPTION*)pushStruct(L, structData, sizeof(FMOD_DSP_DESCRIPTION), FMODExt_registry_FMOD_DSP_DESCRIPTION))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_DESCRIPTION
#define FMODExt_check_ptr_FMOD_DSP_DESCRIPTION(L, index) ((FMOD_DSP_DESCRIPTION*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_DESCRIPTION, "FMOD_DSP_DESCRIPTION"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_DESCRIPTION
FMODExt_propertyOverride_FMOD_DSP_DESCRIPTION
#else
declarePropertyGetter(ptr_FMOD_DSP_DESCRIPTION, FMOD_DSP_DESCRIPTION*);
declarePropertyGetterPtr(FMOD_DSP_DESCRIPTION, FMOD_DSP_DESCRIPTION);
declarePropertySetterPtr(FMOD_DSP_DESCRIPTION, FMOD_DSP_DESCRIPTION);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_STATE_DFT_FUNCTIONS
#define FMODExt_push_ptr_FMOD_DSP_STATE_DFT_FUNCTIONS(L, structData) ((FMOD_DSP_STATE_DFT_FUNCTIONS*)pushStruct(L, structData, sizeof(FMOD_DSP_STATE_DFT_FUNCTIONS), FMODExt_registry_FMOD_DSP_STATE_DFT_FUNCTIONS))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_STATE_DFT_FUNCTIONS
#define FMODExt_check_ptr_FMOD_DSP_STATE_DFT_FUNCTIONS(L, index) ((FMOD_DSP_STATE_DFT_FUNCTIONS*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_STATE_DFT_FUNCTIONS, "FMOD_DSP_STATE_DFT_FUNCTIONS"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_STATE_DFT_FUNCTIONS
FMODExt_propertyOverride_FMOD_DSP_STATE_DFT_FUNCTIONS
#else
declarePropertyGetter(ptr_FMOD_DSP_STATE_DFT_FUNCTIONS, FMOD_DSP_STATE_DFT_FUNCTIONS*);
declarePropertyGetterPtr(FMOD_DSP_STATE_DFT_FUNCTIONS, FMOD_DSP_STATE_DFT_FUNCTIONS);
declarePropertySetterPtr(FMOD_DSP_STATE_DFT_FUNCTIONS, FMOD_DSP_STATE_DFT_FUNCTIONS);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_STATE_PAN_FUNCTIONS
#define FMODExt_push_ptr_FMOD_DSP_STATE_PAN_FUNCTIONS(L, structData) ((FMOD_DSP_STATE_PAN_FUNCTIONS*)pushStruct(L, structData, sizeof(FMOD_DSP_STATE_PAN_FUNCTIONS), FMODExt_registry_FMOD_DSP_STATE_PAN_FUNCTIONS))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_STATE_PAN_FUNCTIONS
#define FMODExt_check_ptr_FMOD_DSP_STATE_PAN_FUNCTIONS(L, index) ((FMOD_DSP_STATE_PAN_FUNCTIONS*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_STATE_PAN_FUNCTIONS, "FMOD_DSP_STATE_PAN_FUNCTIONS"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_STATE_PAN_FUNCTIONS
FMODExt_propertyOverride_FMOD_DSP_STATE_PAN_FUNCTIONS
#else
declarePropertyGetter(ptr_FMOD_DSP_STATE_PAN_FUNCTIONS, FMOD_DSP_STATE_PAN_FUNCTIONS*);
declarePropertyGetterPtr(FMOD_DSP_STATE_PAN_FUNCTIONS, FMOD_DSP_STATE_PAN_FUNCTIONS);
declarePropertySetterPtr(FMOD_DSP_STATE_PAN_FUNCTIONS, FMOD_DSP_STATE_PAN_FUNCTIONS);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_STATE_FUNCTIONS
#define FMODExt_push_ptr_FMOD_DSP_STATE_FUNCTIONS(L, structData) ((FMOD_DSP_STATE_FUNCTIONS*)pushStruct(L, structData, sizeof(FMOD_DSP_STATE_FUNCTIONS), FMODExt_registry_FMOD_DSP_STATE_FUNCTIONS))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_STATE_FUNCTIONS
#define FMODExt_check_ptr_FMOD_DSP_STATE_FUNCTIONS(L, index) ((FMOD_DSP_STATE_FUNCTIONS*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_STATE_FUNCTIONS, "FMOD_DSP_STATE_FUNCTIONS"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_STATE_FUNCTIONS
FMODExt_propertyOverride_FMOD_DSP_STATE_FUNCTIONS
#else
declarePropertyGetter(ptr_FMOD_DSP_STATE_FUNCTIONS, FMOD_DSP_STATE_FUNCTIONS*);
declarePropertyGetterPtr(FMOD_DSP_STATE_FUNCTIONS, FMOD_DSP_STATE_FUNCTIONS);
declarePropertySetterPtr(FMOD_DSP_STATE_FUNCTIONS, FMOD_DSP_STATE_FUNCTIONS);
#endif

#ifndef FMODExt_push_ptr_FMOD_DSP_METERING_INFO
#define FMODExt_push_ptr_FMOD_DSP_METERING_INFO(L, structData) ((FMOD_DSP_METERING_INFO*)pushStruct(L, structData, sizeof(FMOD_DSP_METERING_INFO), FMODExt_registry_FMOD_DSP_METERING_INFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_DSP_METERING_INFO
#define FMODExt_check_ptr_FMOD_DSP_METERING_INFO(L, index) ((FMOD_DSP_METERING_INFO*)checkStruct(L, index, FMODExt_registry_FMOD_DSP_METERING_INFO, "FMOD_DSP_METERING_INFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_DSP_METERING_INFO
FMODExt_propertyOverride_FMOD_DSP_METERING_INFO
#else
declarePropertyGetter(ptr_FMOD_DSP_METERING_INFO, FMOD_DSP_METERING_INFO*);
declarePropertyGetterPtr(FMOD_DSP_METERING_INFO, FMOD_DSP_METERING_INFO);
declarePropertySetterPtr(FMOD_DSP_METERING_INFO, FMOD_DSP_METERING_INFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_OUTPUT_STATE
#define FMODExt_push_ptr_FMOD_OUTPUT_STATE(L, structData) ((FMOD_OUTPUT_STATE*)pushStruct(L, structData, sizeof(FMOD_OUTPUT_STATE), FMODExt_registry_FMOD_OUTPUT_STATE))
#endif
#ifndef FMODExt_check_ptr_FMOD_OUTPUT_STATE
#define FMODExt_check_ptr_FMOD_OUTPUT_STATE(L, index) ((FMOD_OUTPUT_STATE*)checkStruct(L, index, FMODExt_registry_FMOD_OUTPUT_STATE, "FMOD_OUTPUT_STATE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_OUTPUT_STATE
FMODExt_propertyOverride_FMOD_OUTPUT_STATE
#else
declarePropertyGetter(ptr_FMOD_OUTPUT_STATE, FMOD_OUTPUT_STATE*);
declarePropertyGetterPtr(FMOD_OUTPUT_STATE, FMOD_OUTPUT_STATE);
declarePropertySetterPtr(FMOD_OUTPUT_STATE, FMOD_OUTPUT_STATE);
#endif

#ifndef FMODExt_push_ptr_FMOD_OUTPUT_OBJECT3DINFO
#define FMODExt_push_ptr_FMOD_OUTPUT_OBJECT3DINFO(L, structData) ((FMOD_OUTPUT_OBJECT3DINFO*)pushStruct(L, structData, sizeof(FMOD_OUTPUT_OBJECT3DINFO), FMODExt_registry_FMOD_OUTPUT_OBJECT3DINFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_OUTPUT_OBJECT3DINFO
#define FMODExt_check_ptr_FMOD_OUTPUT_OBJECT3DINFO(L, index) ((FMOD_OUTPUT_OBJECT3DINFO*)checkStruct(L, index, FMODExt_registry_FMOD_OUTPUT_OBJECT3DINFO, "FMOD_OUTPUT_OBJECT3DINFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_OUTPUT_OBJECT3DINFO
FMODExt_propertyOverride_FMOD_OUTPUT_OBJECT3DINFO
#else
declarePropertyGetter(ptr_FMOD_OUTPUT_OBJECT3DINFO, FMOD_OUTPUT_OBJECT3DINFO*);
declarePropertyGetterPtr(FMOD_OUTPUT_OBJECT3DINFO, FMOD_OUTPUT_OBJECT3DINFO);
declarePropertySetterPtr(FMOD_OUTPUT_OBJECT3DINFO, FMOD_OUTPUT_OBJECT3DINFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_OUTPUT_DESCRIPTION
#define FMODExt_push_ptr_FMOD_OUTPUT_DESCRIPTION(L, structData) ((FMOD_OUTPUT_DESCRIPTION*)pushStruct(L, structData, sizeof(FMOD_OUTPUT_DESCRIPTION), FMODExt_registry_FMOD_OUTPUT_DESCRIPTION))
#endif
#ifndef FMODExt_check_ptr_FMOD_OUTPUT_DESCRIPTION
#define FMODExt_check_ptr_FMOD_OUTPUT_DESCRIPTION(L, index) ((FMOD_OUTPUT_DESCRIPTION*)checkStruct(L, index, FMODExt_registry_FMOD_OUTPUT_DESCRIPTION, "FMOD_OUTPUT_DESCRIPTION"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_OUTPUT_DESCRIPTION
FMODExt_propertyOverride_FMOD_OUTPUT_DESCRIPTION
#else
declarePropertyGetter(ptr_FMOD_OUTPUT_DESCRIPTION, FMOD_OUTPUT_DESCRIPTION*);
declarePropertyGetterPtr(FMOD_OUTPUT_DESCRIPTION, FMOD_OUTPUT_DESCRIPTION);
declarePropertySetterPtr(FMOD_OUTPUT_DESCRIPTION, FMOD_OUTPUT_DESCRIPTION);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_SYSTEM
#define FMODExt_push_ptr_FMOD_STUDIO_SYSTEM(L, instance) ((FMOD_STUDIO_SYSTEM*)pushClass(L, instance, FMODExt_registry_FMOD_STUDIO_SYSTEM))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_SYSTEM
#define FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, index) ((FMOD_STUDIO_SYSTEM*)checkClass(L, index, FMODExt_registry_FMOD_STUDIO_SYSTEM, "FMOD_STUDIO_SYSTEM"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_SYSTEM
FMODExt_propertyOverride_FMOD_STUDIO_SYSTEM
#else
declarePropertyGetter(ptr_FMOD_STUDIO_SYSTEM, FMOD_STUDIO_SYSTEM*);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_EVENTDESCRIPTION
#define FMODExt_push_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, instance) ((FMOD_STUDIO_EVENTDESCRIPTION*)pushClass(L, instance, FMODExt_registry_FMOD_STUDIO_EVENTDESCRIPTION))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION
#define FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, index) ((FMOD_STUDIO_EVENTDESCRIPTION*)checkClass(L, index, FMODExt_registry_FMOD_STUDIO_EVENTDESCRIPTION, "FMOD_STUDIO_EVENTDESCRIPTION"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_EVENTDESCRIPTION
FMODExt_propertyOverride_FMOD_STUDIO_EVENTDESCRIPTION
#else
declarePropertyGetter(ptr_FMOD_STUDIO_EVENTDESCRIPTION, FMOD_STUDIO_EVENTDESCRIPTION*);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_EVENTINSTANCE
#define FMODExt_push_ptr_FMOD_STUDIO_EVENTINSTANCE(L, instance) ((FMOD_STUDIO_EVENTINSTANCE*)pushClassRefCount(L, instance, FMODExt_registry_FMOD_STUDIO_EVENTINSTANCE))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE
#define FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, index) ((FMOD_STUDIO_EVENTINSTANCE*)checkClass(L, index, FMODExt_registry_FMOD_STUDIO_EVENTINSTANCE, "FMOD_STUDIO_EVENTINSTANCE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_EVENTINSTANCE
FMODExt_propertyOverride_FMOD_STUDIO_EVENTINSTANCE
#else
declarePropertyGetter(ptr_FMOD_STUDIO_EVENTINSTANCE, FMOD_STUDIO_EVENTINSTANCE*);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_BUS
#define FMODExt_push_ptr_FMOD_STUDIO_BUS(L, instance) ((FMOD_STUDIO_BUS*)pushClass(L, instance, FMODExt_registry_FMOD_STUDIO_BUS))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_BUS
#define FMODExt_check_ptr_FMOD_STUDIO_BUS(L, index) ((FMOD_STUDIO_BUS*)checkClass(L, index, FMODExt_registry_FMOD_STUDIO_BUS, "FMOD_STUDIO_BUS"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_BUS
FMODExt_propertyOverride_FMOD_STUDIO_BUS
#else
declarePropertyGetter(ptr_FMOD_STUDIO_BUS, FMOD_STUDIO_BUS*);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_VCA
#define FMODExt_push_ptr_FMOD_STUDIO_VCA(L, instance) ((FMOD_STUDIO_VCA*)pushClass(L, instance, FMODExt_registry_FMOD_STUDIO_VCA))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_VCA
#define FMODExt_check_ptr_FMOD_STUDIO_VCA(L, index) ((FMOD_STUDIO_VCA*)checkClass(L, index, FMODExt_registry_FMOD_STUDIO_VCA, "FMOD_STUDIO_VCA"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_VCA
FMODExt_propertyOverride_FMOD_STUDIO_VCA
#else
declarePropertyGetter(ptr_FMOD_STUDIO_VCA, FMOD_STUDIO_VCA*);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_BANK
#define FMODExt_push_ptr_FMOD_STUDIO_BANK(L, instance) ((FMOD_STUDIO_BANK*)pushClass(L, instance, FMODExt_registry_FMOD_STUDIO_BANK))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_BANK
#define FMODExt_check_ptr_FMOD_STUDIO_BANK(L, index) ((FMOD_STUDIO_BANK*)checkClass(L, index, FMODExt_registry_FMOD_STUDIO_BANK, "FMOD_STUDIO_BANK"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_BANK
FMODExt_propertyOverride_FMOD_STUDIO_BANK
#else
declarePropertyGetter(ptr_FMOD_STUDIO_BANK, FMOD_STUDIO_BANK*);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_COMMANDREPLAY
#define FMODExt_push_ptr_FMOD_STUDIO_COMMANDREPLAY(L, instance) ((FMOD_STUDIO_COMMANDREPLAY*)pushClass(L, instance, FMODExt_registry_FMOD_STUDIO_COMMANDREPLAY))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY
#define FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, index) ((FMOD_STUDIO_COMMANDREPLAY*)checkClass(L, index, FMODExt_registry_FMOD_STUDIO_COMMANDREPLAY, "FMOD_STUDIO_COMMANDREPLAY"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_COMMANDREPLAY
FMODExt_propertyOverride_FMOD_STUDIO_COMMANDREPLAY
#else
declarePropertyGetter(ptr_FMOD_STUDIO_COMMANDREPLAY, FMOD_STUDIO_COMMANDREPLAY*);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_BANK_INFO
#define FMODExt_push_ptr_FMOD_STUDIO_BANK_INFO(L, structData) ((FMOD_STUDIO_BANK_INFO*)pushStruct(L, structData, sizeof(FMOD_STUDIO_BANK_INFO), FMODExt_registry_FMOD_STUDIO_BANK_INFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_BANK_INFO
#define FMODExt_check_ptr_FMOD_STUDIO_BANK_INFO(L, index) ((FMOD_STUDIO_BANK_INFO*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_BANK_INFO, "FMOD_STUDIO_BANK_INFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_BANK_INFO
FMODExt_propertyOverride_FMOD_STUDIO_BANK_INFO
#else
declarePropertyGetter(ptr_FMOD_STUDIO_BANK_INFO, FMOD_STUDIO_BANK_INFO*);
declarePropertyGetterPtr(FMOD_STUDIO_BANK_INFO, FMOD_STUDIO_BANK_INFO);
declarePropertySetterPtr(FMOD_STUDIO_BANK_INFO, FMOD_STUDIO_BANK_INFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_ID
#define FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_ID(L, structData) ((FMOD_STUDIO_PARAMETER_ID*)pushStruct(L, structData, sizeof(FMOD_STUDIO_PARAMETER_ID), FMODExt_registry_FMOD_STUDIO_PARAMETER_ID))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID
#define FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, index) ((FMOD_STUDIO_PARAMETER_ID*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_PARAMETER_ID, "FMOD_STUDIO_PARAMETER_ID"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_PARAMETER_ID
FMODExt_propertyOverride_FMOD_STUDIO_PARAMETER_ID
#else
declarePropertyGetter(ptr_FMOD_STUDIO_PARAMETER_ID, FMOD_STUDIO_PARAMETER_ID*);
declarePropertyGetterPtr(FMOD_STUDIO_PARAMETER_ID, FMOD_STUDIO_PARAMETER_ID);
declarePropertySetterPtr(FMOD_STUDIO_PARAMETER_ID, FMOD_STUDIO_PARAMETER_ID);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION
#define FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION(L, structData) ((FMOD_STUDIO_PARAMETER_DESCRIPTION*)pushStruct(L, structData, sizeof(FMOD_STUDIO_PARAMETER_DESCRIPTION), FMODExt_registry_FMOD_STUDIO_PARAMETER_DESCRIPTION))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION
#define FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION(L, index) ((FMOD_STUDIO_PARAMETER_DESCRIPTION*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_PARAMETER_DESCRIPTION, "FMOD_STUDIO_PARAMETER_DESCRIPTION"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_PARAMETER_DESCRIPTION
FMODExt_propertyOverride_FMOD_STUDIO_PARAMETER_DESCRIPTION
#else
declarePropertyGetter(ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION, FMOD_STUDIO_PARAMETER_DESCRIPTION*);
declarePropertyGetterPtr(FMOD_STUDIO_PARAMETER_DESCRIPTION, FMOD_STUDIO_PARAMETER_DESCRIPTION);
declarePropertySetterPtr(FMOD_STUDIO_PARAMETER_DESCRIPTION, FMOD_STUDIO_PARAMETER_DESCRIPTION);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_USER_PROPERTY
#define FMODExt_push_ptr_FMOD_STUDIO_USER_PROPERTY(L, structData) ((FMOD_STUDIO_USER_PROPERTY*)pushStruct(L, structData, sizeof(FMOD_STUDIO_USER_PROPERTY), FMODExt_registry_FMOD_STUDIO_USER_PROPERTY))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_USER_PROPERTY
#define FMODExt_check_ptr_FMOD_STUDIO_USER_PROPERTY(L, index) ((FMOD_STUDIO_USER_PROPERTY*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_USER_PROPERTY, "FMOD_STUDIO_USER_PROPERTY"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_USER_PROPERTY
FMODExt_propertyOverride_FMOD_STUDIO_USER_PROPERTY
#else
declarePropertyGetter(ptr_FMOD_STUDIO_USER_PROPERTY, FMOD_STUDIO_USER_PROPERTY*);
declarePropertyGetterPtr(FMOD_STUDIO_USER_PROPERTY, FMOD_STUDIO_USER_PROPERTY);
declarePropertySetterPtr(FMOD_STUDIO_USER_PROPERTY, FMOD_STUDIO_USER_PROPERTY);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES
#define FMODExt_push_ptr_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES(L, structData) ((FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES*)pushStruct(L, structData, sizeof(FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES), FMODExt_registry_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES
#define FMODExt_check_ptr_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES(L, index) ((FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES, "FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES
FMODExt_propertyOverride_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES
#else
declarePropertyGetter(ptr_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES, FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES*);
declarePropertyGetterPtr(FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES, FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES);
declarePropertySetterPtr(FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES, FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES
#define FMODExt_push_ptr_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES(L, structData) ((FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES*)pushStruct(L, structData, sizeof(FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES), FMODExt_registry_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES
#define FMODExt_check_ptr_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES(L, index) ((FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES, "FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES
FMODExt_propertyOverride_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES
#else
declarePropertyGetter(ptr_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES, FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES*);
declarePropertyGetterPtr(FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES, FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES);
declarePropertySetterPtr(FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES, FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES
#define FMODExt_push_ptr_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES(L, structData) ((FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES*)pushStruct(L, structData, sizeof(FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES), FMODExt_registry_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES
#define FMODExt_check_ptr_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES(L, index) ((FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES, "FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES
FMODExt_propertyOverride_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES
#else
declarePropertyGetter(ptr_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES, FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES*);
declarePropertyGetterPtr(FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES, FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES);
declarePropertySetterPtr(FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES, FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES
#define FMODExt_push_ptr_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES(L, structData) ((FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES*)pushStruct(L, structData, sizeof(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES), FMODExt_registry_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES
#define FMODExt_check_ptr_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES(L, index) ((FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, "FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES
FMODExt_propertyOverride_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES
#else
declarePropertyGetter(ptr_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES*);
declarePropertyGetterPtr(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES);
declarePropertySetterPtr(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES
#define FMODExt_push_ptr_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES(L, structData) ((FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES*)pushStruct(L, structData, sizeof(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES), FMODExt_registry_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES
#define FMODExt_check_ptr_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES(L, index) ((FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES, "FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES
FMODExt_propertyOverride_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES
#else
declarePropertyGetter(ptr_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES, FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES*);
declarePropertyGetterPtr(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES, FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES);
declarePropertySetterPtr(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES, FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_ADVANCEDSETTINGS
#define FMODExt_push_ptr_FMOD_STUDIO_ADVANCEDSETTINGS(L, structData) ((FMOD_STUDIO_ADVANCEDSETTINGS*)pushStruct(L, structData, sizeof(FMOD_STUDIO_ADVANCEDSETTINGS), FMODExt_registry_FMOD_STUDIO_ADVANCEDSETTINGS))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_ADVANCEDSETTINGS
#define FMODExt_check_ptr_FMOD_STUDIO_ADVANCEDSETTINGS(L, index) ((FMOD_STUDIO_ADVANCEDSETTINGS*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_ADVANCEDSETTINGS, "FMOD_STUDIO_ADVANCEDSETTINGS"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_ADVANCEDSETTINGS
FMODExt_propertyOverride_FMOD_STUDIO_ADVANCEDSETTINGS
#else
declarePropertyGetter(ptr_FMOD_STUDIO_ADVANCEDSETTINGS, FMOD_STUDIO_ADVANCEDSETTINGS*);
declarePropertyGetterPtr(FMOD_STUDIO_ADVANCEDSETTINGS, FMOD_STUDIO_ADVANCEDSETTINGS);
declarePropertySetterPtr(FMOD_STUDIO_ADVANCEDSETTINGS, FMOD_STUDIO_ADVANCEDSETTINGS);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_CPU_USAGE
#define FMODExt_push_ptr_FMOD_STUDIO_CPU_USAGE(L, structData) ((FMOD_STUDIO_CPU_USAGE*)pushStruct(L, structData, sizeof(FMOD_STUDIO_CPU_USAGE), FMODExt_registry_FMOD_STUDIO_CPU_USAGE))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_CPU_USAGE
#define FMODExt_check_ptr_FMOD_STUDIO_CPU_USAGE(L, index) ((FMOD_STUDIO_CPU_USAGE*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_CPU_USAGE, "FMOD_STUDIO_CPU_USAGE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_CPU_USAGE
FMODExt_propertyOverride_FMOD_STUDIO_CPU_USAGE
#else
declarePropertyGetter(ptr_FMOD_STUDIO_CPU_USAGE, FMOD_STUDIO_CPU_USAGE*);
declarePropertyGetterPtr(FMOD_STUDIO_CPU_USAGE, FMOD_STUDIO_CPU_USAGE);
declarePropertySetterPtr(FMOD_STUDIO_CPU_USAGE, FMOD_STUDIO_CPU_USAGE);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_BUFFER_INFO
#define FMODExt_push_ptr_FMOD_STUDIO_BUFFER_INFO(L, structData) ((FMOD_STUDIO_BUFFER_INFO*)pushStruct(L, structData, sizeof(FMOD_STUDIO_BUFFER_INFO), FMODExt_registry_FMOD_STUDIO_BUFFER_INFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_BUFFER_INFO
#define FMODExt_check_ptr_FMOD_STUDIO_BUFFER_INFO(L, index) ((FMOD_STUDIO_BUFFER_INFO*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_BUFFER_INFO, "FMOD_STUDIO_BUFFER_INFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_BUFFER_INFO
FMODExt_propertyOverride_FMOD_STUDIO_BUFFER_INFO
#else
declarePropertyGetter(ptr_FMOD_STUDIO_BUFFER_INFO, FMOD_STUDIO_BUFFER_INFO*);
declarePropertyGetterPtr(FMOD_STUDIO_BUFFER_INFO, FMOD_STUDIO_BUFFER_INFO);
declarePropertySetterPtr(FMOD_STUDIO_BUFFER_INFO, FMOD_STUDIO_BUFFER_INFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_BUFFER_USAGE
#define FMODExt_push_ptr_FMOD_STUDIO_BUFFER_USAGE(L, structData) ((FMOD_STUDIO_BUFFER_USAGE*)pushStruct(L, structData, sizeof(FMOD_STUDIO_BUFFER_USAGE), FMODExt_registry_FMOD_STUDIO_BUFFER_USAGE))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_BUFFER_USAGE
#define FMODExt_check_ptr_FMOD_STUDIO_BUFFER_USAGE(L, index) ((FMOD_STUDIO_BUFFER_USAGE*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_BUFFER_USAGE, "FMOD_STUDIO_BUFFER_USAGE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_BUFFER_USAGE
FMODExt_propertyOverride_FMOD_STUDIO_BUFFER_USAGE
#else
declarePropertyGetter(ptr_FMOD_STUDIO_BUFFER_USAGE, FMOD_STUDIO_BUFFER_USAGE*);
declarePropertyGetterPtr(FMOD_STUDIO_BUFFER_USAGE, FMOD_STUDIO_BUFFER_USAGE);
declarePropertySetterPtr(FMOD_STUDIO_BUFFER_USAGE, FMOD_STUDIO_BUFFER_USAGE);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_SOUND_INFO
#define FMODExt_push_ptr_FMOD_STUDIO_SOUND_INFO(L, structData) ((FMOD_STUDIO_SOUND_INFO*)pushStruct(L, structData, sizeof(FMOD_STUDIO_SOUND_INFO), FMODExt_registry_FMOD_STUDIO_SOUND_INFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_SOUND_INFO
#define FMODExt_check_ptr_FMOD_STUDIO_SOUND_INFO(L, index) ((FMOD_STUDIO_SOUND_INFO*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_SOUND_INFO, "FMOD_STUDIO_SOUND_INFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_SOUND_INFO
FMODExt_propertyOverride_FMOD_STUDIO_SOUND_INFO
#else
declarePropertyGetter(ptr_FMOD_STUDIO_SOUND_INFO, FMOD_STUDIO_SOUND_INFO*);
declarePropertyGetterPtr(FMOD_STUDIO_SOUND_INFO, FMOD_STUDIO_SOUND_INFO);
declarePropertySetterPtr(FMOD_STUDIO_SOUND_INFO, FMOD_STUDIO_SOUND_INFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_COMMAND_INFO
#define FMODExt_push_ptr_FMOD_STUDIO_COMMAND_INFO(L, structData) ((FMOD_STUDIO_COMMAND_INFO*)pushStruct(L, structData, sizeof(FMOD_STUDIO_COMMAND_INFO), FMODExt_registry_FMOD_STUDIO_COMMAND_INFO))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_COMMAND_INFO
#define FMODExt_check_ptr_FMOD_STUDIO_COMMAND_INFO(L, index) ((FMOD_STUDIO_COMMAND_INFO*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_COMMAND_INFO, "FMOD_STUDIO_COMMAND_INFO"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_COMMAND_INFO
FMODExt_propertyOverride_FMOD_STUDIO_COMMAND_INFO
#else
declarePropertyGetter(ptr_FMOD_STUDIO_COMMAND_INFO, FMOD_STUDIO_COMMAND_INFO*);
declarePropertyGetterPtr(FMOD_STUDIO_COMMAND_INFO, FMOD_STUDIO_COMMAND_INFO);
declarePropertySetterPtr(FMOD_STUDIO_COMMAND_INFO, FMOD_STUDIO_COMMAND_INFO);
#endif

#ifndef FMODExt_push_ptr_FMOD_STUDIO_MEMORY_USAGE
#define FMODExt_push_ptr_FMOD_STUDIO_MEMORY_USAGE(L, structData) ((FMOD_STUDIO_MEMORY_USAGE*)pushStruct(L, structData, sizeof(FMOD_STUDIO_MEMORY_USAGE), FMODExt_registry_FMOD_STUDIO_MEMORY_USAGE))
#endif
#ifndef FMODExt_check_ptr_FMOD_STUDIO_MEMORY_USAGE
#define FMODExt_check_ptr_FMOD_STUDIO_MEMORY_USAGE(L, index) ((FMOD_STUDIO_MEMORY_USAGE*)checkStruct(L, index, FMODExt_registry_FMOD_STUDIO_MEMORY_USAGE, "FMOD_STUDIO_MEMORY_USAGE"))
#endif
#ifdef FMODExt_propertyOverride_FMOD_STUDIO_MEMORY_USAGE
FMODExt_propertyOverride_FMOD_STUDIO_MEMORY_USAGE
#else
declarePropertyGetter(ptr_FMOD_STUDIO_MEMORY_USAGE, FMOD_STUDIO_MEMORY_USAGE*);
declarePropertyGetterPtr(FMOD_STUDIO_MEMORY_USAGE, FMOD_STUDIO_MEMORY_USAGE);
declarePropertySetterPtr(FMOD_STUDIO_MEMORY_USAGE, FMOD_STUDIO_MEMORY_USAGE);
#endif


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

    ensure(ST, FMOD_Studio_System_LoadBankMemory, FMOD_RESULT, FMOD_STUDIO_SYSTEM *system, const char *buffer, int length, FMOD_STUDIO_LOAD_MEMORY_MODE mode, FMOD_STUDIO_LOAD_BANK_FLAGS flags, FMOD_STUDIO_BANK **bank);
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
    ensure(ST, FMOD_Studio_System_SetListenerAttributes, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, int, const FMOD_3D_ATTRIBUTES*, const FMOD_VECTOR*);
    errCheck(FMOD_Studio_System_SetListenerAttributes(system, index, attributes, hasAttenuationPosition ? &attenuationposition : NULL));
    return 0;
}

#define FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes _FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes
static int _FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_3D_ATTRIBUTES* attributes = FMODExt_check_ptr_FMOD_3D_ATTRIBUTES(L, 2);
    ensure(ST, FMOD_Studio_EventInstance_Set3DAttributes, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_3D_ATTRIBUTES*);
    errCheck(FMOD_Studio_EventInstance_Set3DAttributes(eventinstance, attributes));
    return 0;
}

// TODO: FMOD_Sound_Set3DCustomRolloff
// TODO: FMOD_Sound_Get3DCustomRolloff
// TODO: FMOD_Channel_Set3DCustomRolloff
// TODO: FMOD_Channel_Get3DCustomRolloff
// TODO: FMOD_ChannelGroup_Set3DCustomRolloff
// TODO: FMOD_ChannelGroup_Get3DCustomRolloff

#define FMODExt_func_FMOD_DSP_GetParameterFloat _FMODExt_func_FMOD_DSP_GetParameterFloat
static int _FMODExt_func_FMOD_DSP_GetParameterFloat(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    float value;
    char valuestr[FMOD_DSP_GETPARAM_VALUESTR_LENGTH];
    ensure(LL, FMOD_DSP_GetParameterFloat, FMOD_RESULT, FMOD_DSP*, int, float*, char*, int);
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
    ensure(LL, FMOD_DSP_GetParameterInt, FMOD_RESULT, FMOD_DSP*, int, int*, char*, int);
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
    ensure(LL, FMOD_DSP_GetParameterBool, FMOD_RESULT, FMOD_DSP*, int, FMOD_BOOL*, char*, int);
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
    ensure(LL, FMOD_System_GetDriverInfo, FMOD_RESULT, FMOD_SYSTEM*, int, char*, int, FMOD_GUID*, int*, FMOD_SPEAKERMODE*, int*);
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
    ensure(LL, FMOD_System_GetPluginInfo, FMOD_RESULT, FMOD_SYSTEM*, unsigned int, FMOD_PLUGINTYPE*, char*, int, unsigned int*);
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
    ensure(LL, FMOD_System_GetRecordDriverInfo, FMOD_RESULT, FMOD_SYSTEM*, int, char*, int, FMOD_GUID*, int*, FMOD_SPEAKERMODE*, int*, unsigned int*);
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
    ensure(LL, fname, FMOD_RESULT, type1*, char*, int); \
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
    unsigned int offsettype = FMODExt_check_unsigned_int(L, 4);
    ensure(LL, FMOD_Sound_GetSyncPointInfo, FMOD_RESULT, FMOD_SOUND*, FMOD_SYNCPOINT*, char*, int, unsigned int*, unsigned int);
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
    ensure(LL, FMOD_DSP_GetInfo, FMOD_RESULT, FMOD_DSP*, char*, unsigned int*, int*, int*, int*);
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
    ensure(ST, FMOD_Studio_System_LookupPath, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const FMOD_GUID*, char*, int, int*);
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
    ensure(ST, fname, FMOD_RESULT, type1*, char*, int, int*); \
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
    ensure(ST, FMOD_Studio_Bank_GetStringInfo, FMOD_RESULT, FMOD_STUDIO_BANK*, int, FMOD_GUID*, char*, int, int*);
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
    ensure(ST, FMOD_Studio_CommandReplay_GetCommandString, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, int, char*, int);
    errCheck(FMOD_Studio_CommandReplay_GetCommandString(replay, commandindex, buffer, 512));
    lua_pushstring(L, buffer);
    return 1;
}

/* Generated function definitions */


/* FMOD_Memory_Initialize(unknown void* poolmem, input int poollen, unknown FMOD_MEMORY_ALLOC_CALLBACK useralloc, unknown FMOD_MEMORY_REALLOC_CALLBACK userrealloc, unknown FMOD_MEMORY_FREE_CALLBACK userfree, input FMOD_MEMORY_TYPE memtypeflags, ) */


/* FMOD_Memory_GetStats(output int* currentalloced, output int* maxalloced, input FMOD_BOOL blocking, ) */
#ifndef FMODExt_func_FMOD_Memory_GetStats
#define FMODExt_func_FMOD_Memory_GetStats _FMODExt_func_FMOD_Memory_GetStats
static int _FMODExt_func_FMOD_Memory_GetStats(lua_State *L) {
    int currentalloced;
    int maxalloced;
    FMOD_BOOL blocking = FMODExt_check_FMOD_BOOL(L, 1);
    ensure(LL, FMOD_Memory_GetStats, FMOD_RESULT, int*, int*, FMOD_BOOL);
    errCheck(FMOD_Memory_GetStats(&currentalloced, &maxalloced, blocking));
    FMODExt_push_int(L, currentalloced);
    FMODExt_push_int(L, maxalloced);
    return 2;
}
#endif


/* FMOD_Debug_Initialize(input FMOD_DEBUG_FLAGS flags, input FMOD_DEBUG_MODE mode, unknown FMOD_DEBUG_CALLBACK callback, input const char* filename, ) */


/* FMOD_File_SetDiskBusy(input int busy, ) */
#ifndef FMODExt_func_FMOD_File_SetDiskBusy
#define FMODExt_func_FMOD_File_SetDiskBusy _FMODExt_func_FMOD_File_SetDiskBusy
static int _FMODExt_func_FMOD_File_SetDiskBusy(lua_State *L) {
    int busy = FMODExt_check_int(L, 1);
    ensure(LL, FMOD_File_SetDiskBusy, FMOD_RESULT, int);
    errCheck(FMOD_File_SetDiskBusy(busy));
    return 0;
}
#endif


/* FMOD_File_GetDiskBusy(output int* busy, ) */
#ifndef FMODExt_func_FMOD_File_GetDiskBusy
#define FMODExt_func_FMOD_File_GetDiskBusy _FMODExt_func_FMOD_File_GetDiskBusy
static int _FMODExt_func_FMOD_File_GetDiskBusy(lua_State *L) {
    int busy;
    ensure(LL, FMOD_File_GetDiskBusy, FMOD_RESULT, int*);
    errCheck(FMOD_File_GetDiskBusy(&busy));
    FMODExt_push_int(L, busy);
    return 1;
}
#endif


/* FMOD_Thread_SetAttributes(input FMOD_THREAD_TYPE type, input FMOD_THREAD_AFFINITY affinity, input FMOD_THREAD_PRIORITY priority, input FMOD_THREAD_STACK_SIZE stacksize, ) */
#ifndef FMODExt_func_FMOD_Thread_SetAttributes
#define FMODExt_func_FMOD_Thread_SetAttributes _FMODExt_func_FMOD_Thread_SetAttributes
static int _FMODExt_func_FMOD_Thread_SetAttributes(lua_State *L) {
    FMOD_THREAD_TYPE type = FMODExt_check_FMOD_THREAD_TYPE(L, 1);
    FMOD_THREAD_AFFINITY affinity = FMODExt_check_long_long(L, 2);
    FMOD_THREAD_PRIORITY priority = FMODExt_check_int(L, 3);
    FMOD_THREAD_STACK_SIZE stacksize = FMODExt_check_unsigned_int(L, 4);
    ensure(LL, FMOD_Thread_SetAttributes, FMOD_RESULT, FMOD_THREAD_TYPE, FMOD_THREAD_AFFINITY, FMOD_THREAD_PRIORITY, FMOD_THREAD_STACK_SIZE);
    errCheck(FMOD_Thread_SetAttributes(type, affinity, priority, stacksize));
    return 0;
}
#endif


/* FMOD_System_Create(output FMOD_SYSTEM** system, input unsigned int headerversion, ) */
#ifndef FMODExt_func_FMOD_System_Create
#define FMODExt_func_FMOD_System_Create _FMODExt_func_FMOD_System_Create
static int _FMODExt_func_FMOD_System_Create(lua_State *L) {
    FMOD_SYSTEM* system;
    unsigned int headerversion = FMODExt_check_unsigned_int(L, 1);
    ensure(LL, FMOD_System_Create, FMOD_RESULT, FMOD_SYSTEM**, unsigned int);
    errCheck(FMOD_System_Create(&system, headerversion));
    FMODExt_push_ptr_FMOD_SYSTEM(L, system);
    return 1;
}
#endif


/* FMOD_System_Release(input FMOD_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_System_Release
#define FMODExt_func_FMOD_System_Release _FMODExt_func_FMOD_System_Release
static int _FMODExt_func_FMOD_System_Release(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    ensure(LL, FMOD_System_Release, FMOD_RESULT, FMOD_SYSTEM*);
    errCheck(FMOD_System_Release(system));
    return 0;
}
#endif


/* FMOD_System_SetOutput(input FMOD_SYSTEM* system, input FMOD_OUTPUTTYPE output, ) */
#ifndef FMODExt_func_FMOD_System_SetOutput
#define FMODExt_func_FMOD_System_SetOutput _FMODExt_func_FMOD_System_SetOutput
static int _FMODExt_func_FMOD_System_SetOutput(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_OUTPUTTYPE output = FMODExt_check_FMOD_OUTPUTTYPE(L, 2);
    ensure(LL, FMOD_System_SetOutput, FMOD_RESULT, FMOD_SYSTEM*, FMOD_OUTPUTTYPE);
    errCheck(FMOD_System_SetOutput(system, output));
    return 0;
}
#endif


/* FMOD_System_GetOutput(input FMOD_SYSTEM* system, output FMOD_OUTPUTTYPE* output, ) */
#ifndef FMODExt_func_FMOD_System_GetOutput
#define FMODExt_func_FMOD_System_GetOutput _FMODExt_func_FMOD_System_GetOutput
static int _FMODExt_func_FMOD_System_GetOutput(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_OUTPUTTYPE output;
    ensure(LL, FMOD_System_GetOutput, FMOD_RESULT, FMOD_SYSTEM*, FMOD_OUTPUTTYPE*);
    errCheck(FMOD_System_GetOutput(system, &output));
    FMODExt_push_FMOD_OUTPUTTYPE(L, output);
    return 1;
}
#endif


/* FMOD_System_GetNumDrivers(input FMOD_SYSTEM* system, output int* numdrivers, ) */
#ifndef FMODExt_func_FMOD_System_GetNumDrivers
#define FMODExt_func_FMOD_System_GetNumDrivers _FMODExt_func_FMOD_System_GetNumDrivers
static int _FMODExt_func_FMOD_System_GetNumDrivers(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int numdrivers;
    ensure(LL, FMOD_System_GetNumDrivers, FMOD_RESULT, FMOD_SYSTEM*, int*);
    errCheck(FMOD_System_GetNumDrivers(system, &numdrivers));
    FMODExt_push_int(L, numdrivers);
    return 1;
}
#endif


/* FMOD_System_GetDriverInfo(input FMOD_SYSTEM* system, input int id, unknown char* name, input int namelen, output_ptr FMOD_GUID* guid, output int* systemrate, output FMOD_SPEAKERMODE* speakermode, output int* speakermodechannels, ) */


/* FMOD_System_SetDriver(input FMOD_SYSTEM* system, input int driver, ) */
#ifndef FMODExt_func_FMOD_System_SetDriver
#define FMODExt_func_FMOD_System_SetDriver _FMODExt_func_FMOD_System_SetDriver
static int _FMODExt_func_FMOD_System_SetDriver(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int driver = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_System_SetDriver, FMOD_RESULT, FMOD_SYSTEM*, int);
    errCheck(FMOD_System_SetDriver(system, driver));
    return 0;
}
#endif


/* FMOD_System_GetDriver(input FMOD_SYSTEM* system, output int* driver, ) */
#ifndef FMODExt_func_FMOD_System_GetDriver
#define FMODExt_func_FMOD_System_GetDriver _FMODExt_func_FMOD_System_GetDriver
static int _FMODExt_func_FMOD_System_GetDriver(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int driver;
    ensure(LL, FMOD_System_GetDriver, FMOD_RESULT, FMOD_SYSTEM*, int*);
    errCheck(FMOD_System_GetDriver(system, &driver));
    FMODExt_push_int(L, driver);
    return 1;
}
#endif


/* FMOD_System_SetSoftwareChannels(input FMOD_SYSTEM* system, input int numsoftwarechannels, ) */
#ifndef FMODExt_func_FMOD_System_SetSoftwareChannels
#define FMODExt_func_FMOD_System_SetSoftwareChannels _FMODExt_func_FMOD_System_SetSoftwareChannels
static int _FMODExt_func_FMOD_System_SetSoftwareChannels(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int numsoftwarechannels = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_System_SetSoftwareChannels, FMOD_RESULT, FMOD_SYSTEM*, int);
    errCheck(FMOD_System_SetSoftwareChannels(system, numsoftwarechannels));
    return 0;
}
#endif


/* FMOD_System_GetSoftwareChannels(input FMOD_SYSTEM* system, output int* numsoftwarechannels, ) */
#ifndef FMODExt_func_FMOD_System_GetSoftwareChannels
#define FMODExt_func_FMOD_System_GetSoftwareChannels _FMODExt_func_FMOD_System_GetSoftwareChannels
static int _FMODExt_func_FMOD_System_GetSoftwareChannels(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int numsoftwarechannels;
    ensure(LL, FMOD_System_GetSoftwareChannels, FMOD_RESULT, FMOD_SYSTEM*, int*);
    errCheck(FMOD_System_GetSoftwareChannels(system, &numsoftwarechannels));
    FMODExt_push_int(L, numsoftwarechannels);
    return 1;
}
#endif


/* FMOD_System_SetSoftwareFormat(input FMOD_SYSTEM* system, input int samplerate, input FMOD_SPEAKERMODE speakermode, input int numrawspeakers, ) */
#ifndef FMODExt_func_FMOD_System_SetSoftwareFormat
#define FMODExt_func_FMOD_System_SetSoftwareFormat _FMODExt_func_FMOD_System_SetSoftwareFormat
static int _FMODExt_func_FMOD_System_SetSoftwareFormat(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int samplerate = FMODExt_check_int(L, 2);
    FMOD_SPEAKERMODE speakermode = FMODExt_check_FMOD_SPEAKERMODE(L, 3);
    int numrawspeakers = FMODExt_check_int(L, 4);
    ensure(LL, FMOD_System_SetSoftwareFormat, FMOD_RESULT, FMOD_SYSTEM*, int, FMOD_SPEAKERMODE, int);
    errCheck(FMOD_System_SetSoftwareFormat(system, samplerate, speakermode, numrawspeakers));
    return 0;
}
#endif


/* FMOD_System_GetSoftwareFormat(input FMOD_SYSTEM* system, output int* samplerate, output FMOD_SPEAKERMODE* speakermode, output int* numrawspeakers, ) */
#ifndef FMODExt_func_FMOD_System_GetSoftwareFormat
#define FMODExt_func_FMOD_System_GetSoftwareFormat _FMODExt_func_FMOD_System_GetSoftwareFormat
static int _FMODExt_func_FMOD_System_GetSoftwareFormat(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int samplerate;
    FMOD_SPEAKERMODE speakermode;
    int numrawspeakers;
    ensure(LL, FMOD_System_GetSoftwareFormat, FMOD_RESULT, FMOD_SYSTEM*, int*, FMOD_SPEAKERMODE*, int*);
    errCheck(FMOD_System_GetSoftwareFormat(system, &samplerate, &speakermode, &numrawspeakers));
    FMODExt_push_int(L, samplerate);
    FMODExt_push_FMOD_SPEAKERMODE(L, speakermode);
    FMODExt_push_int(L, numrawspeakers);
    return 3;
}
#endif


/* FMOD_System_SetDSPBufferSize(input FMOD_SYSTEM* system, input unsigned int bufferlength, input int numbuffers, ) */
#ifndef FMODExt_func_FMOD_System_SetDSPBufferSize
#define FMODExt_func_FMOD_System_SetDSPBufferSize _FMODExt_func_FMOD_System_SetDSPBufferSize
static int _FMODExt_func_FMOD_System_SetDSPBufferSize(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int bufferlength = FMODExt_check_unsigned_int(L, 2);
    int numbuffers = FMODExt_check_int(L, 3);
    ensure(LL, FMOD_System_SetDSPBufferSize, FMOD_RESULT, FMOD_SYSTEM*, unsigned int, int);
    errCheck(FMOD_System_SetDSPBufferSize(system, bufferlength, numbuffers));
    return 0;
}
#endif


/* FMOD_System_GetDSPBufferSize(input FMOD_SYSTEM* system, output unsigned int* bufferlength, output int* numbuffers, ) */
#ifndef FMODExt_func_FMOD_System_GetDSPBufferSize
#define FMODExt_func_FMOD_System_GetDSPBufferSize _FMODExt_func_FMOD_System_GetDSPBufferSize
static int _FMODExt_func_FMOD_System_GetDSPBufferSize(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int bufferlength;
    int numbuffers;
    ensure(LL, FMOD_System_GetDSPBufferSize, FMOD_RESULT, FMOD_SYSTEM*, unsigned int*, int*);
    errCheck(FMOD_System_GetDSPBufferSize(system, &bufferlength, &numbuffers));
    FMODExt_push_unsigned_int(L, bufferlength);
    FMODExt_push_int(L, numbuffers);
    return 2;
}
#endif


/* FMOD_System_SetFileSystem(input FMOD_SYSTEM* system, unknown FMOD_FILE_OPEN_CALLBACK useropen, unknown FMOD_FILE_CLOSE_CALLBACK userclose, unknown FMOD_FILE_READ_CALLBACK userread, unknown FMOD_FILE_SEEK_CALLBACK userseek, unknown FMOD_FILE_ASYNCREAD_CALLBACK userasyncread, unknown FMOD_FILE_ASYNCCANCEL_CALLBACK userasynccancel, input int blockalign, ) */


/* FMOD_System_AttachFileSystem(input FMOD_SYSTEM* system, unknown FMOD_FILE_OPEN_CALLBACK useropen, unknown FMOD_FILE_CLOSE_CALLBACK userclose, unknown FMOD_FILE_READ_CALLBACK userread, unknown FMOD_FILE_SEEK_CALLBACK userseek, ) */


/* FMOD_System_SetAdvancedSettings(input FMOD_SYSTEM* system, output_ptr FMOD_ADVANCEDSETTINGS* settings, ) */
#ifndef FMODExt_func_FMOD_System_SetAdvancedSettings
#define FMODExt_func_FMOD_System_SetAdvancedSettings _FMODExt_func_FMOD_System_SetAdvancedSettings
static int _FMODExt_func_FMOD_System_SetAdvancedSettings(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_ADVANCEDSETTINGS* settings = FMODExt_push_ptr_FMOD_ADVANCEDSETTINGS(L, NULL);
    ensure(LL, FMOD_System_SetAdvancedSettings, FMOD_RESULT, FMOD_SYSTEM*, FMOD_ADVANCEDSETTINGS*);
    errCheck(FMOD_System_SetAdvancedSettings(system, settings));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_System_GetAdvancedSettings(input FMOD_SYSTEM* system, output_ptr FMOD_ADVANCEDSETTINGS* settings, ) */
#ifndef FMODExt_func_FMOD_System_GetAdvancedSettings
#define FMODExt_func_FMOD_System_GetAdvancedSettings _FMODExt_func_FMOD_System_GetAdvancedSettings
static int _FMODExt_func_FMOD_System_GetAdvancedSettings(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_ADVANCEDSETTINGS* settings = FMODExt_push_ptr_FMOD_ADVANCEDSETTINGS(L, NULL);
    ensure(LL, FMOD_System_GetAdvancedSettings, FMOD_RESULT, FMOD_SYSTEM*, FMOD_ADVANCEDSETTINGS*);
    errCheck(FMOD_System_GetAdvancedSettings(system, settings));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_System_SetCallback(input FMOD_SYSTEM* system, unknown FMOD_SYSTEM_CALLBACK callback, input FMOD_SYSTEM_CALLBACK_TYPE callbackmask, ) */


/* FMOD_System_SetPluginPath(input FMOD_SYSTEM* system, input const char* path, ) */
#ifndef FMODExt_func_FMOD_System_SetPluginPath
#define FMODExt_func_FMOD_System_SetPluginPath _FMODExt_func_FMOD_System_SetPluginPath
static int _FMODExt_func_FMOD_System_SetPluginPath(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const char* path = FMODExt_check_ptr_char(L, 2);
    ensure(LL, FMOD_System_SetPluginPath, FMOD_RESULT, FMOD_SYSTEM*, const char*);
    errCheck(FMOD_System_SetPluginPath(system, path));
    return 0;
}
#endif


/* FMOD_System_LoadPlugin(input FMOD_SYSTEM* system, input const char* filename, output unsigned int* handle, input unsigned int priority, ) */
#ifndef FMODExt_func_FMOD_System_LoadPlugin
#define FMODExt_func_FMOD_System_LoadPlugin _FMODExt_func_FMOD_System_LoadPlugin
static int _FMODExt_func_FMOD_System_LoadPlugin(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const char* filename = FMODExt_check_ptr_char(L, 2);
    unsigned int handle;
    unsigned int priority = FMODExt_check_unsigned_int(L, 3);
    ensure(LL, FMOD_System_LoadPlugin, FMOD_RESULT, FMOD_SYSTEM*, const char*, unsigned int*, unsigned int);
    errCheck(FMOD_System_LoadPlugin(system, filename, &handle, priority));
    FMODExt_push_unsigned_int(L, handle);
    return 1;
}
#endif


/* FMOD_System_UnloadPlugin(input FMOD_SYSTEM* system, input unsigned int handle, ) */
#ifndef FMODExt_func_FMOD_System_UnloadPlugin
#define FMODExt_func_FMOD_System_UnloadPlugin _FMODExt_func_FMOD_System_UnloadPlugin
static int _FMODExt_func_FMOD_System_UnloadPlugin(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int handle = FMODExt_check_unsigned_int(L, 2);
    ensure(LL, FMOD_System_UnloadPlugin, FMOD_RESULT, FMOD_SYSTEM*, unsigned int);
    errCheck(FMOD_System_UnloadPlugin(system, handle));
    return 0;
}
#endif


/* FMOD_System_GetNumNestedPlugins(input FMOD_SYSTEM* system, input unsigned int handle, output int* count, ) */
#ifndef FMODExt_func_FMOD_System_GetNumNestedPlugins
#define FMODExt_func_FMOD_System_GetNumNestedPlugins _FMODExt_func_FMOD_System_GetNumNestedPlugins
static int _FMODExt_func_FMOD_System_GetNumNestedPlugins(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int handle = FMODExt_check_unsigned_int(L, 2);
    int count;
    ensure(LL, FMOD_System_GetNumNestedPlugins, FMOD_RESULT, FMOD_SYSTEM*, unsigned int, int*);
    errCheck(FMOD_System_GetNumNestedPlugins(system, handle, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_System_GetNestedPlugin(input FMOD_SYSTEM* system, input unsigned int handle, input int index, output unsigned int* nestedhandle, ) */
#ifndef FMODExt_func_FMOD_System_GetNestedPlugin
#define FMODExt_func_FMOD_System_GetNestedPlugin _FMODExt_func_FMOD_System_GetNestedPlugin
static int _FMODExt_func_FMOD_System_GetNestedPlugin(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int handle = FMODExt_check_unsigned_int(L, 2);
    int index = FMODExt_check_int(L, 3);
    unsigned int nestedhandle;
    ensure(LL, FMOD_System_GetNestedPlugin, FMOD_RESULT, FMOD_SYSTEM*, unsigned int, int, unsigned int*);
    errCheck(FMOD_System_GetNestedPlugin(system, handle, index, &nestedhandle));
    FMODExt_push_unsigned_int(L, nestedhandle);
    return 1;
}
#endif


/* FMOD_System_GetNumPlugins(input FMOD_SYSTEM* system, input FMOD_PLUGINTYPE plugintype, output int* numplugins, ) */
#ifndef FMODExt_func_FMOD_System_GetNumPlugins
#define FMODExt_func_FMOD_System_GetNumPlugins _FMODExt_func_FMOD_System_GetNumPlugins
static int _FMODExt_func_FMOD_System_GetNumPlugins(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_PLUGINTYPE plugintype = FMODExt_check_FMOD_PLUGINTYPE(L, 2);
    int numplugins;
    ensure(LL, FMOD_System_GetNumPlugins, FMOD_RESULT, FMOD_SYSTEM*, FMOD_PLUGINTYPE, int*);
    errCheck(FMOD_System_GetNumPlugins(system, plugintype, &numplugins));
    FMODExt_push_int(L, numplugins);
    return 1;
}
#endif


/* FMOD_System_GetPluginHandle(input FMOD_SYSTEM* system, input FMOD_PLUGINTYPE plugintype, input int index, output unsigned int* handle, ) */
#ifndef FMODExt_func_FMOD_System_GetPluginHandle
#define FMODExt_func_FMOD_System_GetPluginHandle _FMODExt_func_FMOD_System_GetPluginHandle
static int _FMODExt_func_FMOD_System_GetPluginHandle(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_PLUGINTYPE plugintype = FMODExt_check_FMOD_PLUGINTYPE(L, 2);
    int index = FMODExt_check_int(L, 3);
    unsigned int handle;
    ensure(LL, FMOD_System_GetPluginHandle, FMOD_RESULT, FMOD_SYSTEM*, FMOD_PLUGINTYPE, int, unsigned int*);
    errCheck(FMOD_System_GetPluginHandle(system, plugintype, index, &handle));
    FMODExt_push_unsigned_int(L, handle);
    return 1;
}
#endif


/* FMOD_System_GetPluginInfo(input FMOD_SYSTEM* system, input unsigned int handle, output FMOD_PLUGINTYPE* plugintype, unknown char* name, input int namelen, output unsigned int* version, ) */


/* FMOD_System_SetOutputByPlugin(input FMOD_SYSTEM* system, input unsigned int handle, ) */
#ifndef FMODExt_func_FMOD_System_SetOutputByPlugin
#define FMODExt_func_FMOD_System_SetOutputByPlugin _FMODExt_func_FMOD_System_SetOutputByPlugin
static int _FMODExt_func_FMOD_System_SetOutputByPlugin(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int handle = FMODExt_check_unsigned_int(L, 2);
    ensure(LL, FMOD_System_SetOutputByPlugin, FMOD_RESULT, FMOD_SYSTEM*, unsigned int);
    errCheck(FMOD_System_SetOutputByPlugin(system, handle));
    return 0;
}
#endif


/* FMOD_System_GetOutputByPlugin(input FMOD_SYSTEM* system, output unsigned int* handle, ) */
#ifndef FMODExt_func_FMOD_System_GetOutputByPlugin
#define FMODExt_func_FMOD_System_GetOutputByPlugin _FMODExt_func_FMOD_System_GetOutputByPlugin
static int _FMODExt_func_FMOD_System_GetOutputByPlugin(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int handle;
    ensure(LL, FMOD_System_GetOutputByPlugin, FMOD_RESULT, FMOD_SYSTEM*, unsigned int*);
    errCheck(FMOD_System_GetOutputByPlugin(system, &handle));
    FMODExt_push_unsigned_int(L, handle);
    return 1;
}
#endif


/* FMOD_System_CreateDSPByPlugin(input FMOD_SYSTEM* system, input unsigned int handle, output FMOD_DSP** dsp, ) */
#ifndef FMODExt_func_FMOD_System_CreateDSPByPlugin
#define FMODExt_func_FMOD_System_CreateDSPByPlugin _FMODExt_func_FMOD_System_CreateDSPByPlugin
static int _FMODExt_func_FMOD_System_CreateDSPByPlugin(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int handle = FMODExt_check_unsigned_int(L, 2);
    FMOD_DSP* dsp;
    ensure(LL, FMOD_System_CreateDSPByPlugin, FMOD_RESULT, FMOD_SYSTEM*, unsigned int, FMOD_DSP**);
    errCheck(FMOD_System_CreateDSPByPlugin(system, handle, &dsp));
    FMODExt_push_ptr_FMOD_DSP(L, dsp);
    return 1;
}
#endif


/* FMOD_System_GetDSPInfoByPlugin(input FMOD_SYSTEM* system, input unsigned int handle, output const FMOD_DSP_DESCRIPTION** description, ) */
#ifndef FMODExt_func_FMOD_System_GetDSPInfoByPlugin
#define FMODExt_func_FMOD_System_GetDSPInfoByPlugin _FMODExt_func_FMOD_System_GetDSPInfoByPlugin
static int _FMODExt_func_FMOD_System_GetDSPInfoByPlugin(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int handle = FMODExt_check_unsigned_int(L, 2);
    const FMOD_DSP_DESCRIPTION* description;
    ensure(LL, FMOD_System_GetDSPInfoByPlugin, FMOD_RESULT, FMOD_SYSTEM*, unsigned int, const FMOD_DSP_DESCRIPTION**);
    errCheck(FMOD_System_GetDSPInfoByPlugin(system, handle, &description));
    FMODExt_push_ptr_FMOD_DSP_DESCRIPTION(L, description);
    return 1;
}
#endif


/* FMOD_System_RegisterCodec(input FMOD_SYSTEM* system, output_ptr FMOD_CODEC_DESCRIPTION* description, output unsigned int* handle, input unsigned int priority, ) */
#ifndef FMODExt_func_FMOD_System_RegisterCodec
#define FMODExt_func_FMOD_System_RegisterCodec _FMODExt_func_FMOD_System_RegisterCodec
static int _FMODExt_func_FMOD_System_RegisterCodec(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_CODEC_DESCRIPTION* description = FMODExt_push_ptr_FMOD_CODEC_DESCRIPTION(L, NULL);
    unsigned int handle;
    unsigned int priority = FMODExt_check_unsigned_int(L, 2);
    ensure(LL, FMOD_System_RegisterCodec, FMOD_RESULT, FMOD_SYSTEM*, FMOD_CODEC_DESCRIPTION*, unsigned int*, unsigned int);
    errCheck(FMOD_System_RegisterCodec(system, description, &handle, priority));
    lua_pushvalue(L, -1);
    FMODExt_push_unsigned_int(L, handle);
    return 2;
}
#endif


/* FMOD_System_RegisterDSP(input FMOD_SYSTEM* system, input const FMOD_DSP_DESCRIPTION* description, output unsigned int* handle, ) */
#ifndef FMODExt_func_FMOD_System_RegisterDSP
#define FMODExt_func_FMOD_System_RegisterDSP _FMODExt_func_FMOD_System_RegisterDSP
static int _FMODExt_func_FMOD_System_RegisterDSP(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const FMOD_DSP_DESCRIPTION* description = FMODExt_check_ptr_FMOD_DSP_DESCRIPTION(L, 2);
    unsigned int handle;
    ensure(LL, FMOD_System_RegisterDSP, FMOD_RESULT, FMOD_SYSTEM*, const FMOD_DSP_DESCRIPTION*, unsigned int*);
    errCheck(FMOD_System_RegisterDSP(system, description, &handle));
    FMODExt_push_unsigned_int(L, handle);
    return 1;
}
#endif


/* FMOD_System_RegisterOutput(input FMOD_SYSTEM* system, input const FMOD_OUTPUT_DESCRIPTION* description, output unsigned int* handle, ) */
#ifndef FMODExt_func_FMOD_System_RegisterOutput
#define FMODExt_func_FMOD_System_RegisterOutput _FMODExt_func_FMOD_System_RegisterOutput
static int _FMODExt_func_FMOD_System_RegisterOutput(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const FMOD_OUTPUT_DESCRIPTION* description = FMODExt_check_ptr_FMOD_OUTPUT_DESCRIPTION(L, 2);
    unsigned int handle;
    ensure(LL, FMOD_System_RegisterOutput, FMOD_RESULT, FMOD_SYSTEM*, const FMOD_OUTPUT_DESCRIPTION*, unsigned int*);
    errCheck(FMOD_System_RegisterOutput(system, description, &handle));
    FMODExt_push_unsigned_int(L, handle);
    return 1;
}
#endif


/* FMOD_System_Init(input FMOD_SYSTEM* system, input int maxchannels, input FMOD_INITFLAGS flags, unknown void* extradriverdata, ) */


/* FMOD_System_Close(input FMOD_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_System_Close
#define FMODExt_func_FMOD_System_Close _FMODExt_func_FMOD_System_Close
static int _FMODExt_func_FMOD_System_Close(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    ensure(LL, FMOD_System_Close, FMOD_RESULT, FMOD_SYSTEM*);
    errCheck(FMOD_System_Close(system));
    return 0;
}
#endif


/* FMOD_System_Update(input FMOD_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_System_Update
#define FMODExt_func_FMOD_System_Update _FMODExt_func_FMOD_System_Update
static int _FMODExt_func_FMOD_System_Update(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    ensure(LL, FMOD_System_Update, FMOD_RESULT, FMOD_SYSTEM*);
    errCheck(FMOD_System_Update(system));
    return 0;
}
#endif


/* FMOD_System_SetSpeakerPosition(input FMOD_SYSTEM* system, input FMOD_SPEAKER speaker, input float x, input float y, input FMOD_BOOL active, ) */
#ifndef FMODExt_func_FMOD_System_SetSpeakerPosition
#define FMODExt_func_FMOD_System_SetSpeakerPosition _FMODExt_func_FMOD_System_SetSpeakerPosition
static int _FMODExt_func_FMOD_System_SetSpeakerPosition(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_SPEAKER speaker = FMODExt_check_FMOD_SPEAKER(L, 2);
    float x = FMODExt_check_float(L, 3);
    float y = FMODExt_check_float(L, 4);
    FMOD_BOOL active = FMODExt_check_FMOD_BOOL(L, 5);
    ensure(LL, FMOD_System_SetSpeakerPosition, FMOD_RESULT, FMOD_SYSTEM*, FMOD_SPEAKER, float, float, FMOD_BOOL);
    errCheck(FMOD_System_SetSpeakerPosition(system, speaker, x, y, active));
    return 0;
}
#endif


/* FMOD_System_GetSpeakerPosition(input FMOD_SYSTEM* system, input FMOD_SPEAKER speaker, output float* x, output float* y, output FMOD_BOOL* active, ) */
#ifndef FMODExt_func_FMOD_System_GetSpeakerPosition
#define FMODExt_func_FMOD_System_GetSpeakerPosition _FMODExt_func_FMOD_System_GetSpeakerPosition
static int _FMODExt_func_FMOD_System_GetSpeakerPosition(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_SPEAKER speaker = FMODExt_check_FMOD_SPEAKER(L, 2);
    float x;
    float y;
    FMOD_BOOL active;
    ensure(LL, FMOD_System_GetSpeakerPosition, FMOD_RESULT, FMOD_SYSTEM*, FMOD_SPEAKER, float*, float*, FMOD_BOOL*);
    errCheck(FMOD_System_GetSpeakerPosition(system, speaker, &x, &y, &active));
    FMODExt_push_float(L, x);
    FMODExt_push_float(L, y);
    FMODExt_push_FMOD_BOOL(L, active);
    return 3;
}
#endif


/* FMOD_System_SetStreamBufferSize(input FMOD_SYSTEM* system, input unsigned int filebuffersize, input FMOD_TIMEUNIT filebuffersizetype, ) */
#ifndef FMODExt_func_FMOD_System_SetStreamBufferSize
#define FMODExt_func_FMOD_System_SetStreamBufferSize _FMODExt_func_FMOD_System_SetStreamBufferSize
static int _FMODExt_func_FMOD_System_SetStreamBufferSize(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int filebuffersize = FMODExt_check_unsigned_int(L, 2);
    FMOD_TIMEUNIT filebuffersizetype = FMODExt_check_unsigned_int(L, 3);
    ensure(LL, FMOD_System_SetStreamBufferSize, FMOD_RESULT, FMOD_SYSTEM*, unsigned int, FMOD_TIMEUNIT);
    errCheck(FMOD_System_SetStreamBufferSize(system, filebuffersize, filebuffersizetype));
    return 0;
}
#endif


/* FMOD_System_GetStreamBufferSize(input FMOD_SYSTEM* system, output unsigned int* filebuffersize, output FMOD_TIMEUNIT* filebuffersizetype, ) */
#ifndef FMODExt_func_FMOD_System_GetStreamBufferSize
#define FMODExt_func_FMOD_System_GetStreamBufferSize _FMODExt_func_FMOD_System_GetStreamBufferSize
static int _FMODExt_func_FMOD_System_GetStreamBufferSize(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int filebuffersize;
    FMOD_TIMEUNIT filebuffersizetype;
    ensure(LL, FMOD_System_GetStreamBufferSize, FMOD_RESULT, FMOD_SYSTEM*, unsigned int*, FMOD_TIMEUNIT*);
    errCheck(FMOD_System_GetStreamBufferSize(system, &filebuffersize, &filebuffersizetype));
    FMODExt_push_unsigned_int(L, filebuffersize);
    FMODExt_push_unsigned_int(L, filebuffersizetype);
    return 2;
}
#endif


/* FMOD_System_Set3DSettings(input FMOD_SYSTEM* system, input float dopplerscale, input float distancefactor, input float rolloffscale, ) */
#ifndef FMODExt_func_FMOD_System_Set3DSettings
#define FMODExt_func_FMOD_System_Set3DSettings _FMODExt_func_FMOD_System_Set3DSettings
static int _FMODExt_func_FMOD_System_Set3DSettings(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    float dopplerscale = FMODExt_check_float(L, 2);
    float distancefactor = FMODExt_check_float(L, 3);
    float rolloffscale = FMODExt_check_float(L, 4);
    ensure(LL, FMOD_System_Set3DSettings, FMOD_RESULT, FMOD_SYSTEM*, float, float, float);
    errCheck(FMOD_System_Set3DSettings(system, dopplerscale, distancefactor, rolloffscale));
    return 0;
}
#endif


/* FMOD_System_Get3DSettings(input FMOD_SYSTEM* system, output float* dopplerscale, output float* distancefactor, output float* rolloffscale, ) */
#ifndef FMODExt_func_FMOD_System_Get3DSettings
#define FMODExt_func_FMOD_System_Get3DSettings _FMODExt_func_FMOD_System_Get3DSettings
static int _FMODExt_func_FMOD_System_Get3DSettings(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    float dopplerscale;
    float distancefactor;
    float rolloffscale;
    ensure(LL, FMOD_System_Get3DSettings, FMOD_RESULT, FMOD_SYSTEM*, float*, float*, float*);
    errCheck(FMOD_System_Get3DSettings(system, &dopplerscale, &distancefactor, &rolloffscale));
    FMODExt_push_float(L, dopplerscale);
    FMODExt_push_float(L, distancefactor);
    FMODExt_push_float(L, rolloffscale);
    return 3;
}
#endif


/* FMOD_System_Set3DNumListeners(input FMOD_SYSTEM* system, input int numlisteners, ) */
#ifndef FMODExt_func_FMOD_System_Set3DNumListeners
#define FMODExt_func_FMOD_System_Set3DNumListeners _FMODExt_func_FMOD_System_Set3DNumListeners
static int _FMODExt_func_FMOD_System_Set3DNumListeners(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int numlisteners = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_System_Set3DNumListeners, FMOD_RESULT, FMOD_SYSTEM*, int);
    errCheck(FMOD_System_Set3DNumListeners(system, numlisteners));
    return 0;
}
#endif


/* FMOD_System_Get3DNumListeners(input FMOD_SYSTEM* system, output int* numlisteners, ) */
#ifndef FMODExt_func_FMOD_System_Get3DNumListeners
#define FMODExt_func_FMOD_System_Get3DNumListeners _FMODExt_func_FMOD_System_Get3DNumListeners
static int _FMODExt_func_FMOD_System_Get3DNumListeners(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int numlisteners;
    ensure(LL, FMOD_System_Get3DNumListeners, FMOD_RESULT, FMOD_SYSTEM*, int*);
    errCheck(FMOD_System_Get3DNumListeners(system, &numlisteners));
    FMODExt_push_int(L, numlisteners);
    return 1;
}
#endif


/* FMOD_System_Set3DListenerAttributes(input FMOD_SYSTEM* system, input int listener, input_ptr const FMOD_VECTOR* pos, input_ptr const FMOD_VECTOR* vel, input_ptr const FMOD_VECTOR* forward, input_ptr const FMOD_VECTOR* up, ) */
#ifndef FMODExt_func_FMOD_System_Set3DListenerAttributes
#define FMODExt_func_FMOD_System_Set3DListenerAttributes _FMODExt_func_FMOD_System_Set3DListenerAttributes
static int _FMODExt_func_FMOD_System_Set3DListenerAttributes(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int listener = FMODExt_check_int(L, 2);
    const FMOD_VECTOR pos = FMODExt_check_FMOD_VECTOR(L, 3);
    const FMOD_VECTOR vel = FMODExt_check_FMOD_VECTOR(L, 4);
    const FMOD_VECTOR forward = FMODExt_check_FMOD_VECTOR(L, 5);
    const FMOD_VECTOR up = FMODExt_check_FMOD_VECTOR(L, 6);
    ensure(LL, FMOD_System_Set3DListenerAttributes, FMOD_RESULT, FMOD_SYSTEM*, int, const FMOD_VECTOR*, const FMOD_VECTOR*, const FMOD_VECTOR*, const FMOD_VECTOR*);
    errCheck(FMOD_System_Set3DListenerAttributes(system, listener, &pos, &vel, &forward, &up));
    return 0;
}
#endif


/* FMOD_System_Get3DListenerAttributes(input FMOD_SYSTEM* system, input int listener, output FMOD_VECTOR* pos, output FMOD_VECTOR* vel, output FMOD_VECTOR* forward, output FMOD_VECTOR* up, ) */
#ifndef FMODExt_func_FMOD_System_Get3DListenerAttributes
#define FMODExt_func_FMOD_System_Get3DListenerAttributes _FMODExt_func_FMOD_System_Get3DListenerAttributes
static int _FMODExt_func_FMOD_System_Get3DListenerAttributes(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int listener = FMODExt_check_int(L, 2);
    FMOD_VECTOR pos;
    FMOD_VECTOR vel;
    FMOD_VECTOR forward;
    FMOD_VECTOR up;
    ensure(LL, FMOD_System_Get3DListenerAttributes, FMOD_RESULT, FMOD_SYSTEM*, int, FMOD_VECTOR*, FMOD_VECTOR*, FMOD_VECTOR*, FMOD_VECTOR*);
    errCheck(FMOD_System_Get3DListenerAttributes(system, listener, &pos, &vel, &forward, &up));
    FMODExt_push_FMOD_VECTOR(L, pos);
    FMODExt_push_FMOD_VECTOR(L, vel);
    FMODExt_push_FMOD_VECTOR(L, forward);
    FMODExt_push_FMOD_VECTOR(L, up);
    return 4;
}
#endif


/* FMOD_System_Set3DRolloffCallback(input FMOD_SYSTEM* system, unknown FMOD_3D_ROLLOFF_CALLBACK callback, ) */


/* FMOD_System_MixerSuspend(input FMOD_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_System_MixerSuspend
#define FMODExt_func_FMOD_System_MixerSuspend _FMODExt_func_FMOD_System_MixerSuspend
static int _FMODExt_func_FMOD_System_MixerSuspend(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    ensure(LL, FMOD_System_MixerSuspend, FMOD_RESULT, FMOD_SYSTEM*);
    errCheck(FMOD_System_MixerSuspend(system));
    return 0;
}
#endif


/* FMOD_System_MixerResume(input FMOD_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_System_MixerResume
#define FMODExt_func_FMOD_System_MixerResume _FMODExt_func_FMOD_System_MixerResume
static int _FMODExt_func_FMOD_System_MixerResume(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    ensure(LL, FMOD_System_MixerResume, FMOD_RESULT, FMOD_SYSTEM*);
    errCheck(FMOD_System_MixerResume(system));
    return 0;
}
#endif


/* FMOD_System_GetDefaultMixMatrix(input FMOD_SYSTEM* system, input FMOD_SPEAKERMODE sourcespeakermode, input FMOD_SPEAKERMODE targetspeakermode, output float* matrix, input int matrixhop, ) */
#ifndef FMODExt_func_FMOD_System_GetDefaultMixMatrix
#define FMODExt_func_FMOD_System_GetDefaultMixMatrix _FMODExt_func_FMOD_System_GetDefaultMixMatrix
static int _FMODExt_func_FMOD_System_GetDefaultMixMatrix(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_SPEAKERMODE sourcespeakermode = FMODExt_check_FMOD_SPEAKERMODE(L, 2);
    FMOD_SPEAKERMODE targetspeakermode = FMODExt_check_FMOD_SPEAKERMODE(L, 3);
    float matrix;
    int matrixhop = FMODExt_check_int(L, 4);
    ensure(LL, FMOD_System_GetDefaultMixMatrix, FMOD_RESULT, FMOD_SYSTEM*, FMOD_SPEAKERMODE, FMOD_SPEAKERMODE, float*, int);
    errCheck(FMOD_System_GetDefaultMixMatrix(system, sourcespeakermode, targetspeakermode, &matrix, matrixhop));
    FMODExt_push_float(L, matrix);
    return 1;
}
#endif


/* FMOD_System_GetSpeakerModeChannels(input FMOD_SYSTEM* system, input FMOD_SPEAKERMODE mode, output int* channels, ) */
#ifndef FMODExt_func_FMOD_System_GetSpeakerModeChannels
#define FMODExt_func_FMOD_System_GetSpeakerModeChannels _FMODExt_func_FMOD_System_GetSpeakerModeChannels
static int _FMODExt_func_FMOD_System_GetSpeakerModeChannels(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_SPEAKERMODE mode = FMODExt_check_FMOD_SPEAKERMODE(L, 2);
    int channels;
    ensure(LL, FMOD_System_GetSpeakerModeChannels, FMOD_RESULT, FMOD_SYSTEM*, FMOD_SPEAKERMODE, int*);
    errCheck(FMOD_System_GetSpeakerModeChannels(system, mode, &channels));
    FMODExt_push_int(L, channels);
    return 1;
}
#endif


/* FMOD_System_GetVersion(input FMOD_SYSTEM* system, output unsigned int* version, output unsigned int* buildnumber, ) */
#ifndef FMODExt_func_FMOD_System_GetVersion
#define FMODExt_func_FMOD_System_GetVersion _FMODExt_func_FMOD_System_GetVersion
static int _FMODExt_func_FMOD_System_GetVersion(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    unsigned int version;
    unsigned int buildnumber;
    ensure(LL, FMOD_System_GetVersion, FMOD_RESULT, FMOD_SYSTEM*, unsigned int*, unsigned int*);
    errCheck(FMOD_System_GetVersion(system, &version, &buildnumber));
    FMODExt_push_unsigned_int(L, version);
    FMODExt_push_unsigned_int(L, buildnumber);
    return 2;
}
#endif


/* FMOD_System_GetOutputHandle(input FMOD_SYSTEM* system, unknown void** handle, ) */


/* FMOD_System_GetChannelsPlaying(input FMOD_SYSTEM* system, output int* channels, output int* realchannels, ) */
#ifndef FMODExt_func_FMOD_System_GetChannelsPlaying
#define FMODExt_func_FMOD_System_GetChannelsPlaying _FMODExt_func_FMOD_System_GetChannelsPlaying
static int _FMODExt_func_FMOD_System_GetChannelsPlaying(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int channels;
    int realchannels;
    ensure(LL, FMOD_System_GetChannelsPlaying, FMOD_RESULT, FMOD_SYSTEM*, int*, int*);
    errCheck(FMOD_System_GetChannelsPlaying(system, &channels, &realchannels));
    FMODExt_push_int(L, channels);
    FMODExt_push_int(L, realchannels);
    return 2;
}
#endif


/* FMOD_System_GetCPUUsage(input FMOD_SYSTEM* system, output_ptr FMOD_CPU_USAGE* usage, ) */
#ifndef FMODExt_func_FMOD_System_GetCPUUsage
#define FMODExt_func_FMOD_System_GetCPUUsage _FMODExt_func_FMOD_System_GetCPUUsage
static int _FMODExt_func_FMOD_System_GetCPUUsage(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_CPU_USAGE* usage = FMODExt_push_ptr_FMOD_CPU_USAGE(L, NULL);
    ensure(LL, FMOD_System_GetCPUUsage, FMOD_RESULT, FMOD_SYSTEM*, FMOD_CPU_USAGE*);
    errCheck(FMOD_System_GetCPUUsage(system, usage));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_System_GetFileUsage(input FMOD_SYSTEM* system, output long long* sampleBytesRead, output long long* streamBytesRead, output long long* otherBytesRead, ) */
#ifndef FMODExt_func_FMOD_System_GetFileUsage
#define FMODExt_func_FMOD_System_GetFileUsage _FMODExt_func_FMOD_System_GetFileUsage
static int _FMODExt_func_FMOD_System_GetFileUsage(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    long long sampleBytesRead;
    long long streamBytesRead;
    long long otherBytesRead;
    ensure(LL, FMOD_System_GetFileUsage, FMOD_RESULT, FMOD_SYSTEM*, long long*, long long*, long long*);
    errCheck(FMOD_System_GetFileUsage(system, &sampleBytesRead, &streamBytesRead, &otherBytesRead));
    FMODExt_push_long_long(L, sampleBytesRead);
    FMODExt_push_long_long(L, streamBytesRead);
    FMODExt_push_long_long(L, otherBytesRead);
    return 3;
}
#endif


/* FMOD_System_CreateSound(input FMOD_SYSTEM* system, input const char* name_or_data, input FMOD_MODE mode, input FMOD_CREATESOUNDEXINFO* exinfo, output FMOD_SOUND** sound, ) */
#ifndef FMODExt_func_FMOD_System_CreateSound
#define FMODExt_func_FMOD_System_CreateSound _FMODExt_func_FMOD_System_CreateSound
static int _FMODExt_func_FMOD_System_CreateSound(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const char* name_or_data = FMODExt_check_ptr_char(L, 2);
    FMOD_MODE mode = FMODExt_check_unsigned_int(L, 3);
    FMOD_CREATESOUNDEXINFO* exinfo = optional(L, 4, FMOD_CREATESOUNDEXINFO*, FMODExt_check_ptr_FMOD_CREATESOUNDEXINFO(L, 4));
    FMOD_SOUND* sound;
    ensure(LL, FMOD_System_CreateSound, FMOD_RESULT, FMOD_SYSTEM*, const char*, FMOD_MODE, FMOD_CREATESOUNDEXINFO*, FMOD_SOUND**);
    errCheck(FMOD_System_CreateSound(system, name_or_data, mode, exinfo, &sound));
    FMODExt_push_ptr_FMOD_SOUND(L, sound);
    return 1;
}
#endif


/* FMOD_System_CreateStream(input FMOD_SYSTEM* system, input const char* name_or_data, input FMOD_MODE mode, input FMOD_CREATESOUNDEXINFO* exinfo, output FMOD_SOUND** sound, ) */
#ifndef FMODExt_func_FMOD_System_CreateStream
#define FMODExt_func_FMOD_System_CreateStream _FMODExt_func_FMOD_System_CreateStream
static int _FMODExt_func_FMOD_System_CreateStream(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const char* name_or_data = FMODExt_check_ptr_char(L, 2);
    FMOD_MODE mode = FMODExt_check_unsigned_int(L, 3);
    FMOD_CREATESOUNDEXINFO* exinfo = optional(L, 4, FMOD_CREATESOUNDEXINFO*, FMODExt_check_ptr_FMOD_CREATESOUNDEXINFO(L, 4));
    FMOD_SOUND* sound;
    ensure(LL, FMOD_System_CreateStream, FMOD_RESULT, FMOD_SYSTEM*, const char*, FMOD_MODE, FMOD_CREATESOUNDEXINFO*, FMOD_SOUND**);
    errCheck(FMOD_System_CreateStream(system, name_or_data, mode, exinfo, &sound));
    FMODExt_push_ptr_FMOD_SOUND(L, sound);
    return 1;
}
#endif


/* FMOD_System_CreateDSP(input FMOD_SYSTEM* system, input const FMOD_DSP_DESCRIPTION* description, output FMOD_DSP** dsp, ) */
#ifndef FMODExt_func_FMOD_System_CreateDSP
#define FMODExt_func_FMOD_System_CreateDSP _FMODExt_func_FMOD_System_CreateDSP
static int _FMODExt_func_FMOD_System_CreateDSP(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const FMOD_DSP_DESCRIPTION* description = FMODExt_check_ptr_FMOD_DSP_DESCRIPTION(L, 2);
    FMOD_DSP* dsp;
    ensure(LL, FMOD_System_CreateDSP, FMOD_RESULT, FMOD_SYSTEM*, const FMOD_DSP_DESCRIPTION*, FMOD_DSP**);
    errCheck(FMOD_System_CreateDSP(system, description, &dsp));
    FMODExt_push_ptr_FMOD_DSP(L, dsp);
    return 1;
}
#endif


/* FMOD_System_CreateDSPByType(input FMOD_SYSTEM* system, input FMOD_DSP_TYPE type, output FMOD_DSP** dsp, ) */
#ifndef FMODExt_func_FMOD_System_CreateDSPByType
#define FMODExt_func_FMOD_System_CreateDSPByType _FMODExt_func_FMOD_System_CreateDSPByType
static int _FMODExt_func_FMOD_System_CreateDSPByType(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_DSP_TYPE type = FMODExt_check_FMOD_DSP_TYPE(L, 2);
    FMOD_DSP* dsp;
    ensure(LL, FMOD_System_CreateDSPByType, FMOD_RESULT, FMOD_SYSTEM*, FMOD_DSP_TYPE, FMOD_DSP**);
    errCheck(FMOD_System_CreateDSPByType(system, type, &dsp));
    FMODExt_push_ptr_FMOD_DSP(L, dsp);
    return 1;
}
#endif


/* FMOD_System_CreateDSPConnection(input FMOD_SYSTEM* system, input FMOD_DSPCONNECTION_TYPE type, output FMOD_DSPCONNECTION** connection, ) */
#ifndef FMODExt_func_FMOD_System_CreateDSPConnection
#define FMODExt_func_FMOD_System_CreateDSPConnection _FMODExt_func_FMOD_System_CreateDSPConnection
static int _FMODExt_func_FMOD_System_CreateDSPConnection(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_DSPCONNECTION_TYPE type = FMODExt_check_FMOD_DSPCONNECTION_TYPE(L, 2);
    FMOD_DSPCONNECTION* connection;
    ensure(LL, FMOD_System_CreateDSPConnection, FMOD_RESULT, FMOD_SYSTEM*, FMOD_DSPCONNECTION_TYPE, FMOD_DSPCONNECTION**);
    errCheck(FMOD_System_CreateDSPConnection(system, type, &connection));
    FMODExt_push_ptr_FMOD_DSPCONNECTION(L, connection);
    return 1;
}
#endif


/* FMOD_System_CreateChannelGroup(input FMOD_SYSTEM* system, input const char* name, output FMOD_CHANNELGROUP** channelgroup, ) */
#ifndef FMODExt_func_FMOD_System_CreateChannelGroup
#define FMODExt_func_FMOD_System_CreateChannelGroup _FMODExt_func_FMOD_System_CreateChannelGroup
static int _FMODExt_func_FMOD_System_CreateChannelGroup(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    FMOD_CHANNELGROUP* channelgroup;
    ensure(LL, FMOD_System_CreateChannelGroup, FMOD_RESULT, FMOD_SYSTEM*, const char*, FMOD_CHANNELGROUP**);
    errCheck(FMOD_System_CreateChannelGroup(system, name, &channelgroup));
    FMODExt_push_ptr_FMOD_CHANNELGROUP(L, channelgroup);
    return 1;
}
#endif


/* FMOD_System_CreateSoundGroup(input FMOD_SYSTEM* system, input const char* name, output FMOD_SOUNDGROUP** soundgroup, ) */
#ifndef FMODExt_func_FMOD_System_CreateSoundGroup
#define FMODExt_func_FMOD_System_CreateSoundGroup _FMODExt_func_FMOD_System_CreateSoundGroup
static int _FMODExt_func_FMOD_System_CreateSoundGroup(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    FMOD_SOUNDGROUP* soundgroup;
    ensure(LL, FMOD_System_CreateSoundGroup, FMOD_RESULT, FMOD_SYSTEM*, const char*, FMOD_SOUNDGROUP**);
    errCheck(FMOD_System_CreateSoundGroup(system, name, &soundgroup));
    FMODExt_push_ptr_FMOD_SOUNDGROUP(L, soundgroup);
    return 1;
}
#endif


/* FMOD_System_CreateReverb3D(input FMOD_SYSTEM* system, output FMOD_REVERB3D** reverb, ) */
#ifndef FMODExt_func_FMOD_System_CreateReverb3D
#define FMODExt_func_FMOD_System_CreateReverb3D _FMODExt_func_FMOD_System_CreateReverb3D
static int _FMODExt_func_FMOD_System_CreateReverb3D(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_REVERB3D* reverb;
    ensure(LL, FMOD_System_CreateReverb3D, FMOD_RESULT, FMOD_SYSTEM*, FMOD_REVERB3D**);
    errCheck(FMOD_System_CreateReverb3D(system, &reverb));
    FMODExt_push_ptr_FMOD_REVERB3D(L, reverb);
    return 1;
}
#endif


/* FMOD_System_PlaySound(input FMOD_SYSTEM* system, input FMOD_SOUND* sound, input FMOD_CHANNELGROUP* channelgroup, input FMOD_BOOL paused, output FMOD_CHANNEL** channel, ) */
#ifndef FMODExt_func_FMOD_System_PlaySound
#define FMODExt_func_FMOD_System_PlaySound _FMODExt_func_FMOD_System_PlaySound
static int _FMODExt_func_FMOD_System_PlaySound(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 2);
    FMOD_CHANNELGROUP* channelgroup = optional(L, 3, FMOD_CHANNELGROUP*, FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 3));
    FMOD_BOOL paused = optional(L, 4, FMOD_BOOL, FMODExt_check_FMOD_BOOL(L, 4));
    FMOD_CHANNEL* channel;
    ensure(LL, FMOD_System_PlaySound, FMOD_RESULT, FMOD_SYSTEM*, FMOD_SOUND*, FMOD_CHANNELGROUP*, FMOD_BOOL, FMOD_CHANNEL**);
    errCheck(FMOD_System_PlaySound(system, sound, channelgroup, paused, &channel));
    FMODExt_push_ptr_FMOD_CHANNEL(L, channel);
    return 1;
}
#endif


/* FMOD_System_PlayDSP(input FMOD_SYSTEM* system, input FMOD_DSP* dsp, input FMOD_CHANNELGROUP* channelgroup, input FMOD_BOOL paused, output FMOD_CHANNEL** channel, ) */
#ifndef FMODExt_func_FMOD_System_PlayDSP
#define FMODExt_func_FMOD_System_PlayDSP _FMODExt_func_FMOD_System_PlayDSP
static int _FMODExt_func_FMOD_System_PlayDSP(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 2);
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 3);
    FMOD_BOOL paused = FMODExt_check_FMOD_BOOL(L, 4);
    FMOD_CHANNEL* channel;
    ensure(LL, FMOD_System_PlayDSP, FMOD_RESULT, FMOD_SYSTEM*, FMOD_DSP*, FMOD_CHANNELGROUP*, FMOD_BOOL, FMOD_CHANNEL**);
    errCheck(FMOD_System_PlayDSP(system, dsp, channelgroup, paused, &channel));
    FMODExt_push_ptr_FMOD_CHANNEL(L, channel);
    return 1;
}
#endif


/* FMOD_System_GetChannel(input FMOD_SYSTEM* system, input int channelid, output FMOD_CHANNEL** channel, ) */
#ifndef FMODExt_func_FMOD_System_GetChannel
#define FMODExt_func_FMOD_System_GetChannel _FMODExt_func_FMOD_System_GetChannel
static int _FMODExt_func_FMOD_System_GetChannel(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int channelid = FMODExt_check_int(L, 2);
    FMOD_CHANNEL* channel;
    ensure(LL, FMOD_System_GetChannel, FMOD_RESULT, FMOD_SYSTEM*, int, FMOD_CHANNEL**);
    errCheck(FMOD_System_GetChannel(system, channelid, &channel));
    FMODExt_push_ptr_FMOD_CHANNEL(L, channel);
    return 1;
}
#endif


/* FMOD_System_GetDSPInfoByType(input FMOD_SYSTEM* system, input FMOD_DSP_TYPE type, output const FMOD_DSP_DESCRIPTION** description, ) */
#ifndef FMODExt_func_FMOD_System_GetDSPInfoByType
#define FMODExt_func_FMOD_System_GetDSPInfoByType _FMODExt_func_FMOD_System_GetDSPInfoByType
static int _FMODExt_func_FMOD_System_GetDSPInfoByType(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_DSP_TYPE type = FMODExt_check_FMOD_DSP_TYPE(L, 2);
    const FMOD_DSP_DESCRIPTION* description;
    ensure(LL, FMOD_System_GetDSPInfoByType, FMOD_RESULT, FMOD_SYSTEM*, FMOD_DSP_TYPE, const FMOD_DSP_DESCRIPTION**);
    errCheck(FMOD_System_GetDSPInfoByType(system, type, &description));
    FMODExt_push_ptr_FMOD_DSP_DESCRIPTION(L, description);
    return 1;
}
#endif


/* FMOD_System_GetMasterChannelGroup(input FMOD_SYSTEM* system, output FMOD_CHANNELGROUP** channelgroup, ) */
#ifndef FMODExt_func_FMOD_System_GetMasterChannelGroup
#define FMODExt_func_FMOD_System_GetMasterChannelGroup _FMODExt_func_FMOD_System_GetMasterChannelGroup
static int _FMODExt_func_FMOD_System_GetMasterChannelGroup(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_CHANNELGROUP* channelgroup;
    ensure(LL, FMOD_System_GetMasterChannelGroup, FMOD_RESULT, FMOD_SYSTEM*, FMOD_CHANNELGROUP**);
    errCheck(FMOD_System_GetMasterChannelGroup(system, &channelgroup));
    FMODExt_push_ptr_FMOD_CHANNELGROUP(L, channelgroup);
    return 1;
}
#endif


/* FMOD_System_GetMasterSoundGroup(input FMOD_SYSTEM* system, output FMOD_SOUNDGROUP** soundgroup, ) */
#ifndef FMODExt_func_FMOD_System_GetMasterSoundGroup
#define FMODExt_func_FMOD_System_GetMasterSoundGroup _FMODExt_func_FMOD_System_GetMasterSoundGroup
static int _FMODExt_func_FMOD_System_GetMasterSoundGroup(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_SOUNDGROUP* soundgroup;
    ensure(LL, FMOD_System_GetMasterSoundGroup, FMOD_RESULT, FMOD_SYSTEM*, FMOD_SOUNDGROUP**);
    errCheck(FMOD_System_GetMasterSoundGroup(system, &soundgroup));
    FMODExt_push_ptr_FMOD_SOUNDGROUP(L, soundgroup);
    return 1;
}
#endif


/* FMOD_System_AttachChannelGroupToPort(input FMOD_SYSTEM* system, input FMOD_PORT_TYPE portType, input FMOD_PORT_INDEX portIndex, input FMOD_CHANNELGROUP* channelgroup, input FMOD_BOOL passThru, ) */
#ifndef FMODExt_func_FMOD_System_AttachChannelGroupToPort
#define FMODExt_func_FMOD_System_AttachChannelGroupToPort _FMODExt_func_FMOD_System_AttachChannelGroupToPort
static int _FMODExt_func_FMOD_System_AttachChannelGroupToPort(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_PORT_TYPE portType = FMODExt_check_FMOD_PORT_TYPE(L, 2);
    FMOD_PORT_INDEX portIndex = FMODExt_check_unsigned_long_long(L, 3);
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 4);
    FMOD_BOOL passThru = FMODExt_check_FMOD_BOOL(L, 5);
    ensure(LL, FMOD_System_AttachChannelGroupToPort, FMOD_RESULT, FMOD_SYSTEM*, FMOD_PORT_TYPE, FMOD_PORT_INDEX, FMOD_CHANNELGROUP*, FMOD_BOOL);
    errCheck(FMOD_System_AttachChannelGroupToPort(system, portType, portIndex, channelgroup, passThru));
    return 0;
}
#endif


/* FMOD_System_DetachChannelGroupFromPort(input FMOD_SYSTEM* system, input FMOD_CHANNELGROUP* channelgroup, ) */
#ifndef FMODExt_func_FMOD_System_DetachChannelGroupFromPort
#define FMODExt_func_FMOD_System_DetachChannelGroupFromPort _FMODExt_func_FMOD_System_DetachChannelGroupFromPort
static int _FMODExt_func_FMOD_System_DetachChannelGroupFromPort(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 2);
    ensure(LL, FMOD_System_DetachChannelGroupFromPort, FMOD_RESULT, FMOD_SYSTEM*, FMOD_CHANNELGROUP*);
    errCheck(FMOD_System_DetachChannelGroupFromPort(system, channelgroup));
    return 0;
}
#endif


/* FMOD_System_SetReverbProperties(input FMOD_SYSTEM* system, input int instance, input const FMOD_REVERB_PROPERTIES* prop, ) */
#ifndef FMODExt_func_FMOD_System_SetReverbProperties
#define FMODExt_func_FMOD_System_SetReverbProperties _FMODExt_func_FMOD_System_SetReverbProperties
static int _FMODExt_func_FMOD_System_SetReverbProperties(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int instance = FMODExt_check_int(L, 2);
    const FMOD_REVERB_PROPERTIES* prop = FMODExt_check_ptr_FMOD_REVERB_PROPERTIES(L, 3);
    ensure(LL, FMOD_System_SetReverbProperties, FMOD_RESULT, FMOD_SYSTEM*, int, const FMOD_REVERB_PROPERTIES*);
    errCheck(FMOD_System_SetReverbProperties(system, instance, prop));
    return 0;
}
#endif


/* FMOD_System_GetReverbProperties(input FMOD_SYSTEM* system, input int instance, output_ptr FMOD_REVERB_PROPERTIES* prop, ) */
#ifndef FMODExt_func_FMOD_System_GetReverbProperties
#define FMODExt_func_FMOD_System_GetReverbProperties _FMODExt_func_FMOD_System_GetReverbProperties
static int _FMODExt_func_FMOD_System_GetReverbProperties(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int instance = FMODExt_check_int(L, 2);
    FMOD_REVERB_PROPERTIES* prop = FMODExt_push_ptr_FMOD_REVERB_PROPERTIES(L, NULL);
    ensure(LL, FMOD_System_GetReverbProperties, FMOD_RESULT, FMOD_SYSTEM*, int, FMOD_REVERB_PROPERTIES*);
    errCheck(FMOD_System_GetReverbProperties(system, instance, prop));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_System_LockDSP(input FMOD_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_System_LockDSP
#define FMODExt_func_FMOD_System_LockDSP _FMODExt_func_FMOD_System_LockDSP
static int _FMODExt_func_FMOD_System_LockDSP(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    ensure(LL, FMOD_System_LockDSP, FMOD_RESULT, FMOD_SYSTEM*);
    errCheck(FMOD_System_LockDSP(system));
    return 0;
}
#endif


/* FMOD_System_UnlockDSP(input FMOD_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_System_UnlockDSP
#define FMODExt_func_FMOD_System_UnlockDSP _FMODExt_func_FMOD_System_UnlockDSP
static int _FMODExt_func_FMOD_System_UnlockDSP(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    ensure(LL, FMOD_System_UnlockDSP, FMOD_RESULT, FMOD_SYSTEM*);
    errCheck(FMOD_System_UnlockDSP(system));
    return 0;
}
#endif


/* FMOD_System_GetRecordNumDrivers(input FMOD_SYSTEM* system, output int* numdrivers, output int* numconnected, ) */
#ifndef FMODExt_func_FMOD_System_GetRecordNumDrivers
#define FMODExt_func_FMOD_System_GetRecordNumDrivers _FMODExt_func_FMOD_System_GetRecordNumDrivers
static int _FMODExt_func_FMOD_System_GetRecordNumDrivers(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int numdrivers;
    int numconnected;
    ensure(LL, FMOD_System_GetRecordNumDrivers, FMOD_RESULT, FMOD_SYSTEM*, int*, int*);
    errCheck(FMOD_System_GetRecordNumDrivers(system, &numdrivers, &numconnected));
    FMODExt_push_int(L, numdrivers);
    FMODExt_push_int(L, numconnected);
    return 2;
}
#endif


/* FMOD_System_GetRecordDriverInfo(input FMOD_SYSTEM* system, input int id, unknown char* name, input int namelen, output_ptr FMOD_GUID* guid, output int* systemrate, output FMOD_SPEAKERMODE* speakermode, output int* speakermodechannels, output FMOD_DRIVER_STATE* state, ) */


/* FMOD_System_GetRecordPosition(input FMOD_SYSTEM* system, input int id, output unsigned int* position, ) */
#ifndef FMODExt_func_FMOD_System_GetRecordPosition
#define FMODExt_func_FMOD_System_GetRecordPosition _FMODExt_func_FMOD_System_GetRecordPosition
static int _FMODExt_func_FMOD_System_GetRecordPosition(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int id = FMODExt_check_int(L, 2);
    unsigned int position;
    ensure(LL, FMOD_System_GetRecordPosition, FMOD_RESULT, FMOD_SYSTEM*, int, unsigned int*);
    errCheck(FMOD_System_GetRecordPosition(system, id, &position));
    FMODExt_push_unsigned_int(L, position);
    return 1;
}
#endif


/* FMOD_System_RecordStart(input FMOD_SYSTEM* system, input int id, input FMOD_SOUND* sound, input FMOD_BOOL loop, ) */
#ifndef FMODExt_func_FMOD_System_RecordStart
#define FMODExt_func_FMOD_System_RecordStart _FMODExt_func_FMOD_System_RecordStart
static int _FMODExt_func_FMOD_System_RecordStart(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int id = FMODExt_check_int(L, 2);
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 3);
    FMOD_BOOL loop = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(LL, FMOD_System_RecordStart, FMOD_RESULT, FMOD_SYSTEM*, int, FMOD_SOUND*, FMOD_BOOL);
    errCheck(FMOD_System_RecordStart(system, id, sound, loop));
    return 0;
}
#endif


/* FMOD_System_RecordStop(input FMOD_SYSTEM* system, input int id, ) */
#ifndef FMODExt_func_FMOD_System_RecordStop
#define FMODExt_func_FMOD_System_RecordStop _FMODExt_func_FMOD_System_RecordStop
static int _FMODExt_func_FMOD_System_RecordStop(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int id = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_System_RecordStop, FMOD_RESULT, FMOD_SYSTEM*, int);
    errCheck(FMOD_System_RecordStop(system, id));
    return 0;
}
#endif


/* FMOD_System_IsRecording(input FMOD_SYSTEM* system, input int id, output FMOD_BOOL* recording, ) */
#ifndef FMODExt_func_FMOD_System_IsRecording
#define FMODExt_func_FMOD_System_IsRecording _FMODExt_func_FMOD_System_IsRecording
static int _FMODExt_func_FMOD_System_IsRecording(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int id = FMODExt_check_int(L, 2);
    FMOD_BOOL recording;
    ensure(LL, FMOD_System_IsRecording, FMOD_RESULT, FMOD_SYSTEM*, int, FMOD_BOOL*);
    errCheck(FMOD_System_IsRecording(system, id, &recording));
    FMODExt_push_FMOD_BOOL(L, recording);
    return 1;
}
#endif


/* FMOD_System_CreateGeometry(input FMOD_SYSTEM* system, input int maxpolygons, input int maxvertices, output FMOD_GEOMETRY** geometry, ) */
#ifndef FMODExt_func_FMOD_System_CreateGeometry
#define FMODExt_func_FMOD_System_CreateGeometry _FMODExt_func_FMOD_System_CreateGeometry
static int _FMODExt_func_FMOD_System_CreateGeometry(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int maxpolygons = FMODExt_check_int(L, 2);
    int maxvertices = FMODExt_check_int(L, 3);
    FMOD_GEOMETRY* geometry;
    ensure(LL, FMOD_System_CreateGeometry, FMOD_RESULT, FMOD_SYSTEM*, int, int, FMOD_GEOMETRY**);
    errCheck(FMOD_System_CreateGeometry(system, maxpolygons, maxvertices, &geometry));
    FMODExt_push_ptr_FMOD_GEOMETRY(L, geometry);
    return 1;
}
#endif


/* FMOD_System_SetGeometrySettings(input FMOD_SYSTEM* system, input float maxworldsize, ) */
#ifndef FMODExt_func_FMOD_System_SetGeometrySettings
#define FMODExt_func_FMOD_System_SetGeometrySettings _FMODExt_func_FMOD_System_SetGeometrySettings
static int _FMODExt_func_FMOD_System_SetGeometrySettings(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    float maxworldsize = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_System_SetGeometrySettings, FMOD_RESULT, FMOD_SYSTEM*, float);
    errCheck(FMOD_System_SetGeometrySettings(system, maxworldsize));
    return 0;
}
#endif


/* FMOD_System_GetGeometrySettings(input FMOD_SYSTEM* system, output float* maxworldsize, ) */
#ifndef FMODExt_func_FMOD_System_GetGeometrySettings
#define FMODExt_func_FMOD_System_GetGeometrySettings _FMODExt_func_FMOD_System_GetGeometrySettings
static int _FMODExt_func_FMOD_System_GetGeometrySettings(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    float maxworldsize;
    ensure(LL, FMOD_System_GetGeometrySettings, FMOD_RESULT, FMOD_SYSTEM*, float*);
    errCheck(FMOD_System_GetGeometrySettings(system, &maxworldsize));
    FMODExt_push_float(L, maxworldsize);
    return 1;
}
#endif


/* FMOD_System_LoadGeometry(input FMOD_SYSTEM* system, unknown const void* data, input int datasize, output FMOD_GEOMETRY** geometry, ) */


/* FMOD_System_GetGeometryOcclusion(input FMOD_SYSTEM* system, input_ptr const FMOD_VECTOR* listener, input_ptr const FMOD_VECTOR* source, output float* direct, output float* reverb, ) */
#ifndef FMODExt_func_FMOD_System_GetGeometryOcclusion
#define FMODExt_func_FMOD_System_GetGeometryOcclusion _FMODExt_func_FMOD_System_GetGeometryOcclusion
static int _FMODExt_func_FMOD_System_GetGeometryOcclusion(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const FMOD_VECTOR listener = FMODExt_check_FMOD_VECTOR(L, 2);
    const FMOD_VECTOR source = FMODExt_check_FMOD_VECTOR(L, 3);
    float direct;
    float reverb;
    ensure(LL, FMOD_System_GetGeometryOcclusion, FMOD_RESULT, FMOD_SYSTEM*, const FMOD_VECTOR*, const FMOD_VECTOR*, float*, float*);
    errCheck(FMOD_System_GetGeometryOcclusion(system, &listener, &source, &direct, &reverb));
    FMODExt_push_float(L, direct);
    FMODExt_push_float(L, reverb);
    return 2;
}
#endif


/* FMOD_System_SetNetworkProxy(input FMOD_SYSTEM* system, input const char* proxy, ) */
#ifndef FMODExt_func_FMOD_System_SetNetworkProxy
#define FMODExt_func_FMOD_System_SetNetworkProxy _FMODExt_func_FMOD_System_SetNetworkProxy
static int _FMODExt_func_FMOD_System_SetNetworkProxy(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    const char* proxy = FMODExt_check_ptr_char(L, 2);
    ensure(LL, FMOD_System_SetNetworkProxy, FMOD_RESULT, FMOD_SYSTEM*, const char*);
    errCheck(FMOD_System_SetNetworkProxy(system, proxy));
    return 0;
}
#endif


/* FMOD_System_GetNetworkProxy(input FMOD_SYSTEM* system, unknown char* proxy, input int proxylen, ) */


/* FMOD_System_SetNetworkTimeout(input FMOD_SYSTEM* system, input int timeout, ) */
#ifndef FMODExt_func_FMOD_System_SetNetworkTimeout
#define FMODExt_func_FMOD_System_SetNetworkTimeout _FMODExt_func_FMOD_System_SetNetworkTimeout
static int _FMODExt_func_FMOD_System_SetNetworkTimeout(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int timeout = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_System_SetNetworkTimeout, FMOD_RESULT, FMOD_SYSTEM*, int);
    errCheck(FMOD_System_SetNetworkTimeout(system, timeout));
    return 0;
}
#endif


/* FMOD_System_GetNetworkTimeout(input FMOD_SYSTEM* system, output int* timeout, ) */
#ifndef FMODExt_func_FMOD_System_GetNetworkTimeout
#define FMODExt_func_FMOD_System_GetNetworkTimeout _FMODExt_func_FMOD_System_GetNetworkTimeout
static int _FMODExt_func_FMOD_System_GetNetworkTimeout(lua_State *L) {
    FMOD_SYSTEM* system = FMODExt_check_ptr_FMOD_SYSTEM(L, 1);
    int timeout;
    ensure(LL, FMOD_System_GetNetworkTimeout, FMOD_RESULT, FMOD_SYSTEM*, int*);
    errCheck(FMOD_System_GetNetworkTimeout(system, &timeout));
    FMODExt_push_int(L, timeout);
    return 1;
}
#endif


/* FMOD_System_SetUserData(input FMOD_SYSTEM* system, unknown void* userdata, ) */


/* FMOD_System_GetUserData(input FMOD_SYSTEM* system, unknown void** userdata, ) */


/* FMOD_Sound_Release(input FMOD_SOUND* sound, ) */
#ifndef FMODExt_func_FMOD_Sound_Release
#define FMODExt_func_FMOD_Sound_Release _FMODExt_func_FMOD_Sound_Release
static int _FMODExt_func_FMOD_Sound_Release(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    ensure(LL, FMOD_Sound_Release, FMOD_RESULT, FMOD_SOUND*);
    errCheck(FMOD_Sound_Release(sound));
    return 0;
}
#endif


/* FMOD_Sound_GetSystemObject(input FMOD_SOUND* sound, output FMOD_SYSTEM** system, ) */
#ifndef FMODExt_func_FMOD_Sound_GetSystemObject
#define FMODExt_func_FMOD_Sound_GetSystemObject _FMODExt_func_FMOD_Sound_GetSystemObject
static int _FMODExt_func_FMOD_Sound_GetSystemObject(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_SYSTEM* system;
    ensure(LL, FMOD_Sound_GetSystemObject, FMOD_RESULT, FMOD_SOUND*, FMOD_SYSTEM**);
    errCheck(FMOD_Sound_GetSystemObject(sound, &system));
    FMODExt_push_ptr_FMOD_SYSTEM(L, system);
    return 1;
}
#endif


/* FMOD_Sound_Lock(input FMOD_SOUND* sound, input unsigned int offset, input unsigned int length, unknown void** ptr1, unknown void** ptr2, output unsigned int* len1, output unsigned int* len2, ) */


/* FMOD_Sound_Unlock(input FMOD_SOUND* sound, unknown void* ptr1, unknown void* ptr2, input unsigned int len1, input unsigned int len2, ) */


/* FMOD_Sound_SetDefaults(input FMOD_SOUND* sound, input float frequency, input int priority, ) */
#ifndef FMODExt_func_FMOD_Sound_SetDefaults
#define FMODExt_func_FMOD_Sound_SetDefaults _FMODExt_func_FMOD_Sound_SetDefaults
static int _FMODExt_func_FMOD_Sound_SetDefaults(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    float frequency = FMODExt_check_float(L, 2);
    int priority = FMODExt_check_int(L, 3);
    ensure(LL, FMOD_Sound_SetDefaults, FMOD_RESULT, FMOD_SOUND*, float, int);
    errCheck(FMOD_Sound_SetDefaults(sound, frequency, priority));
    return 0;
}
#endif


/* FMOD_Sound_GetDefaults(input FMOD_SOUND* sound, output float* frequency, output int* priority, ) */
#ifndef FMODExt_func_FMOD_Sound_GetDefaults
#define FMODExt_func_FMOD_Sound_GetDefaults _FMODExt_func_FMOD_Sound_GetDefaults
static int _FMODExt_func_FMOD_Sound_GetDefaults(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    float frequency;
    int priority;
    ensure(LL, FMOD_Sound_GetDefaults, FMOD_RESULT, FMOD_SOUND*, float*, int*);
    errCheck(FMOD_Sound_GetDefaults(sound, &frequency, &priority));
    FMODExt_push_float(L, frequency);
    FMODExt_push_int(L, priority);
    return 2;
}
#endif


/* FMOD_Sound_Set3DMinMaxDistance(input FMOD_SOUND* sound, input float min, input float max, ) */
#ifndef FMODExt_func_FMOD_Sound_Set3DMinMaxDistance
#define FMODExt_func_FMOD_Sound_Set3DMinMaxDistance _FMODExt_func_FMOD_Sound_Set3DMinMaxDistance
static int _FMODExt_func_FMOD_Sound_Set3DMinMaxDistance(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    float min = FMODExt_check_float(L, 2);
    float max = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_Sound_Set3DMinMaxDistance, FMOD_RESULT, FMOD_SOUND*, float, float);
    errCheck(FMOD_Sound_Set3DMinMaxDistance(sound, min, max));
    return 0;
}
#endif


/* FMOD_Sound_Get3DMinMaxDistance(input FMOD_SOUND* sound, output float* min, output float* max, ) */
#ifndef FMODExt_func_FMOD_Sound_Get3DMinMaxDistance
#define FMODExt_func_FMOD_Sound_Get3DMinMaxDistance _FMODExt_func_FMOD_Sound_Get3DMinMaxDistance
static int _FMODExt_func_FMOD_Sound_Get3DMinMaxDistance(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    float min;
    float max;
    ensure(LL, FMOD_Sound_Get3DMinMaxDistance, FMOD_RESULT, FMOD_SOUND*, float*, float*);
    errCheck(FMOD_Sound_Get3DMinMaxDistance(sound, &min, &max));
    FMODExt_push_float(L, min);
    FMODExt_push_float(L, max);
    return 2;
}
#endif


/* FMOD_Sound_Set3DConeSettings(input FMOD_SOUND* sound, input float insideconeangle, input float outsideconeangle, input float outsidevolume, ) */
#ifndef FMODExt_func_FMOD_Sound_Set3DConeSettings
#define FMODExt_func_FMOD_Sound_Set3DConeSettings _FMODExt_func_FMOD_Sound_Set3DConeSettings
static int _FMODExt_func_FMOD_Sound_Set3DConeSettings(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    float insideconeangle = FMODExt_check_float(L, 2);
    float outsideconeangle = FMODExt_check_float(L, 3);
    float outsidevolume = FMODExt_check_float(L, 4);
    ensure(LL, FMOD_Sound_Set3DConeSettings, FMOD_RESULT, FMOD_SOUND*, float, float, float);
    errCheck(FMOD_Sound_Set3DConeSettings(sound, insideconeangle, outsideconeangle, outsidevolume));
    return 0;
}
#endif


/* FMOD_Sound_Get3DConeSettings(input FMOD_SOUND* sound, output float* insideconeangle, output float* outsideconeangle, output float* outsidevolume, ) */
#ifndef FMODExt_func_FMOD_Sound_Get3DConeSettings
#define FMODExt_func_FMOD_Sound_Get3DConeSettings _FMODExt_func_FMOD_Sound_Get3DConeSettings
static int _FMODExt_func_FMOD_Sound_Get3DConeSettings(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    float insideconeangle;
    float outsideconeangle;
    float outsidevolume;
    ensure(LL, FMOD_Sound_Get3DConeSettings, FMOD_RESULT, FMOD_SOUND*, float*, float*, float*);
    errCheck(FMOD_Sound_Get3DConeSettings(sound, &insideconeangle, &outsideconeangle, &outsidevolume));
    FMODExt_push_float(L, insideconeangle);
    FMODExt_push_float(L, outsideconeangle);
    FMODExt_push_float(L, outsidevolume);
    return 3;
}
#endif


/* FMOD_Sound_Set3DCustomRolloff(input FMOD_SOUND* sound, output FMOD_VECTOR* points, input int numpoints, ) */
#ifndef FMODExt_func_FMOD_Sound_Set3DCustomRolloff
#define FMODExt_func_FMOD_Sound_Set3DCustomRolloff _FMODExt_func_FMOD_Sound_Set3DCustomRolloff
static int _FMODExt_func_FMOD_Sound_Set3DCustomRolloff(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_VECTOR points;
    int numpoints = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_Sound_Set3DCustomRolloff, FMOD_RESULT, FMOD_SOUND*, FMOD_VECTOR*, int);
    errCheck(FMOD_Sound_Set3DCustomRolloff(sound, &points, numpoints));
    FMODExt_push_FMOD_VECTOR(L, points);
    return 1;
}
#endif


/* FMOD_Sound_Get3DCustomRolloff(input FMOD_SOUND* sound, unknown FMOD_VECTOR** points, output int* numpoints, ) */


/* FMOD_Sound_GetSubSound(input FMOD_SOUND* sound, input int index, output FMOD_SOUND** subsound, ) */
#ifndef FMODExt_func_FMOD_Sound_GetSubSound
#define FMODExt_func_FMOD_Sound_GetSubSound _FMODExt_func_FMOD_Sound_GetSubSound
static int _FMODExt_func_FMOD_Sound_GetSubSound(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_SOUND* subsound;
    ensure(LL, FMOD_Sound_GetSubSound, FMOD_RESULT, FMOD_SOUND*, int, FMOD_SOUND**);
    errCheck(FMOD_Sound_GetSubSound(sound, index, &subsound));
    FMODExt_push_ptr_FMOD_SOUND(L, subsound);
    return 1;
}
#endif


/* FMOD_Sound_GetSubSoundParent(input FMOD_SOUND* sound, output FMOD_SOUND** parentsound, ) */
#ifndef FMODExt_func_FMOD_Sound_GetSubSoundParent
#define FMODExt_func_FMOD_Sound_GetSubSoundParent _FMODExt_func_FMOD_Sound_GetSubSoundParent
static int _FMODExt_func_FMOD_Sound_GetSubSoundParent(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_SOUND* parentsound;
    ensure(LL, FMOD_Sound_GetSubSoundParent, FMOD_RESULT, FMOD_SOUND*, FMOD_SOUND**);
    errCheck(FMOD_Sound_GetSubSoundParent(sound, &parentsound));
    FMODExt_push_ptr_FMOD_SOUND(L, parentsound);
    return 1;
}
#endif


/* FMOD_Sound_GetName(input FMOD_SOUND* sound, unknown char* name, input int namelen, ) */


/* FMOD_Sound_GetLength(input FMOD_SOUND* sound, output unsigned int* length, input FMOD_TIMEUNIT lengthtype, ) */
#ifndef FMODExt_func_FMOD_Sound_GetLength
#define FMODExt_func_FMOD_Sound_GetLength _FMODExt_func_FMOD_Sound_GetLength
static int _FMODExt_func_FMOD_Sound_GetLength(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    unsigned int length;
    FMOD_TIMEUNIT lengthtype = FMODExt_check_unsigned_int(L, 2);
    ensure(LL, FMOD_Sound_GetLength, FMOD_RESULT, FMOD_SOUND*, unsigned int*, FMOD_TIMEUNIT);
    errCheck(FMOD_Sound_GetLength(sound, &length, lengthtype));
    FMODExt_push_unsigned_int(L, length);
    return 1;
}
#endif


/* FMOD_Sound_GetFormat(input FMOD_SOUND* sound, output FMOD_SOUND_TYPE* type, output FMOD_SOUND_FORMAT* format, output int* channels, output int* bits, ) */
#ifndef FMODExt_func_FMOD_Sound_GetFormat
#define FMODExt_func_FMOD_Sound_GetFormat _FMODExt_func_FMOD_Sound_GetFormat
static int _FMODExt_func_FMOD_Sound_GetFormat(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_SOUND_TYPE type;
    FMOD_SOUND_FORMAT format;
    int channels;
    int bits;
    ensure(LL, FMOD_Sound_GetFormat, FMOD_RESULT, FMOD_SOUND*, FMOD_SOUND_TYPE*, FMOD_SOUND_FORMAT*, int*, int*);
    errCheck(FMOD_Sound_GetFormat(sound, &type, &format, &channels, &bits));
    FMODExt_push_FMOD_SOUND_TYPE(L, type);
    FMODExt_push_FMOD_SOUND_FORMAT(L, format);
    FMODExt_push_int(L, channels);
    FMODExt_push_int(L, bits);
    return 4;
}
#endif


/* FMOD_Sound_GetNumSubSounds(input FMOD_SOUND* sound, output int* numsubsounds, ) */
#ifndef FMODExt_func_FMOD_Sound_GetNumSubSounds
#define FMODExt_func_FMOD_Sound_GetNumSubSounds _FMODExt_func_FMOD_Sound_GetNumSubSounds
static int _FMODExt_func_FMOD_Sound_GetNumSubSounds(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int numsubsounds;
    ensure(LL, FMOD_Sound_GetNumSubSounds, FMOD_RESULT, FMOD_SOUND*, int*);
    errCheck(FMOD_Sound_GetNumSubSounds(sound, &numsubsounds));
    FMODExt_push_int(L, numsubsounds);
    return 1;
}
#endif


/* FMOD_Sound_GetNumTags(input FMOD_SOUND* sound, output int* numtags, output int* numtagsupdated, ) */
#ifndef FMODExt_func_FMOD_Sound_GetNumTags
#define FMODExt_func_FMOD_Sound_GetNumTags _FMODExt_func_FMOD_Sound_GetNumTags
static int _FMODExt_func_FMOD_Sound_GetNumTags(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int numtags;
    int numtagsupdated;
    ensure(LL, FMOD_Sound_GetNumTags, FMOD_RESULT, FMOD_SOUND*, int*, int*);
    errCheck(FMOD_Sound_GetNumTags(sound, &numtags, &numtagsupdated));
    FMODExt_push_int(L, numtags);
    FMODExt_push_int(L, numtagsupdated);
    return 2;
}
#endif


/* FMOD_Sound_GetTag(input FMOD_SOUND* sound, input const char* name, input int index, output_ptr FMOD_TAG* tag, ) */
#ifndef FMODExt_func_FMOD_Sound_GetTag
#define FMODExt_func_FMOD_Sound_GetTag _FMODExt_func_FMOD_Sound_GetTag
static int _FMODExt_func_FMOD_Sound_GetTag(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    int index = FMODExt_check_int(L, 3);
    FMOD_TAG* tag = FMODExt_push_ptr_FMOD_TAG(L, NULL);
    ensure(LL, FMOD_Sound_GetTag, FMOD_RESULT, FMOD_SOUND*, const char*, int, FMOD_TAG*);
    errCheck(FMOD_Sound_GetTag(sound, name, index, tag));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Sound_GetOpenState(input FMOD_SOUND* sound, output FMOD_OPENSTATE* openstate, output unsigned int* percentbuffered, output FMOD_BOOL* starving, output FMOD_BOOL* diskbusy, ) */
#ifndef FMODExt_func_FMOD_Sound_GetOpenState
#define FMODExt_func_FMOD_Sound_GetOpenState _FMODExt_func_FMOD_Sound_GetOpenState
static int _FMODExt_func_FMOD_Sound_GetOpenState(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_OPENSTATE openstate;
    unsigned int percentbuffered;
    FMOD_BOOL starving;
    FMOD_BOOL diskbusy;
    ensure(LL, FMOD_Sound_GetOpenState, FMOD_RESULT, FMOD_SOUND*, FMOD_OPENSTATE*, unsigned int*, FMOD_BOOL*, FMOD_BOOL*);
    errCheck(FMOD_Sound_GetOpenState(sound, &openstate, &percentbuffered, &starving, &diskbusy));
    FMODExt_push_FMOD_OPENSTATE(L, openstate);
    FMODExt_push_unsigned_int(L, percentbuffered);
    FMODExt_push_FMOD_BOOL(L, starving);
    FMODExt_push_FMOD_BOOL(L, diskbusy);
    return 4;
}
#endif


/* FMOD_Sound_ReadData(input FMOD_SOUND* sound, unknown void* buffer, input unsigned int length, output unsigned int* read, ) */


/* FMOD_Sound_SeekData(input FMOD_SOUND* sound, input unsigned int pcm, ) */
#ifndef FMODExt_func_FMOD_Sound_SeekData
#define FMODExt_func_FMOD_Sound_SeekData _FMODExt_func_FMOD_Sound_SeekData
static int _FMODExt_func_FMOD_Sound_SeekData(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    unsigned int pcm = FMODExt_check_unsigned_int(L, 2);
    ensure(LL, FMOD_Sound_SeekData, FMOD_RESULT, FMOD_SOUND*, unsigned int);
    errCheck(FMOD_Sound_SeekData(sound, pcm));
    return 0;
}
#endif


/* FMOD_Sound_SetSoundGroup(input FMOD_SOUND* sound, input FMOD_SOUNDGROUP* soundgroup, ) */
#ifndef FMODExt_func_FMOD_Sound_SetSoundGroup
#define FMODExt_func_FMOD_Sound_SetSoundGroup _FMODExt_func_FMOD_Sound_SetSoundGroup
static int _FMODExt_func_FMOD_Sound_SetSoundGroup(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 2);
    ensure(LL, FMOD_Sound_SetSoundGroup, FMOD_RESULT, FMOD_SOUND*, FMOD_SOUNDGROUP*);
    errCheck(FMOD_Sound_SetSoundGroup(sound, soundgroup));
    return 0;
}
#endif


/* FMOD_Sound_GetSoundGroup(input FMOD_SOUND* sound, output FMOD_SOUNDGROUP** soundgroup, ) */
#ifndef FMODExt_func_FMOD_Sound_GetSoundGroup
#define FMODExt_func_FMOD_Sound_GetSoundGroup _FMODExt_func_FMOD_Sound_GetSoundGroup
static int _FMODExt_func_FMOD_Sound_GetSoundGroup(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_SOUNDGROUP* soundgroup;
    ensure(LL, FMOD_Sound_GetSoundGroup, FMOD_RESULT, FMOD_SOUND*, FMOD_SOUNDGROUP**);
    errCheck(FMOD_Sound_GetSoundGroup(sound, &soundgroup));
    FMODExt_push_ptr_FMOD_SOUNDGROUP(L, soundgroup);
    return 1;
}
#endif


/* FMOD_Sound_GetNumSyncPoints(input FMOD_SOUND* sound, output int* numsyncpoints, ) */
#ifndef FMODExt_func_FMOD_Sound_GetNumSyncPoints
#define FMODExt_func_FMOD_Sound_GetNumSyncPoints _FMODExt_func_FMOD_Sound_GetNumSyncPoints
static int _FMODExt_func_FMOD_Sound_GetNumSyncPoints(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int numsyncpoints;
    ensure(LL, FMOD_Sound_GetNumSyncPoints, FMOD_RESULT, FMOD_SOUND*, int*);
    errCheck(FMOD_Sound_GetNumSyncPoints(sound, &numsyncpoints));
    FMODExt_push_int(L, numsyncpoints);
    return 1;
}
#endif


/* FMOD_Sound_GetSyncPoint(input FMOD_SOUND* sound, input int index, output FMOD_SYNCPOINT** point, ) */
#ifndef FMODExt_func_FMOD_Sound_GetSyncPoint
#define FMODExt_func_FMOD_Sound_GetSyncPoint _FMODExt_func_FMOD_Sound_GetSyncPoint
static int _FMODExt_func_FMOD_Sound_GetSyncPoint(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_SYNCPOINT* point;
    ensure(LL, FMOD_Sound_GetSyncPoint, FMOD_RESULT, FMOD_SOUND*, int, FMOD_SYNCPOINT**);
    errCheck(FMOD_Sound_GetSyncPoint(sound, index, &point));
    FMODExt_push_ptr_FMOD_SYNCPOINT(L, point);
    return 1;
}
#endif


/* FMOD_Sound_GetSyncPointInfo(input FMOD_SOUND* sound, input FMOD_SYNCPOINT* point, unknown char* name, input int namelen, output unsigned int* offset, input FMOD_TIMEUNIT offsettype, ) */


/* FMOD_Sound_AddSyncPoint(input FMOD_SOUND* sound, input unsigned int offset, input FMOD_TIMEUNIT offsettype, input const char* name, output FMOD_SYNCPOINT** point, ) */
#ifndef FMODExt_func_FMOD_Sound_AddSyncPoint
#define FMODExt_func_FMOD_Sound_AddSyncPoint _FMODExt_func_FMOD_Sound_AddSyncPoint
static int _FMODExt_func_FMOD_Sound_AddSyncPoint(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    unsigned int offset = FMODExt_check_unsigned_int(L, 2);
    FMOD_TIMEUNIT offsettype = FMODExt_check_unsigned_int(L, 3);
    const char* name = FMODExt_check_ptr_char(L, 4);
    FMOD_SYNCPOINT* point;
    ensure(LL, FMOD_Sound_AddSyncPoint, FMOD_RESULT, FMOD_SOUND*, unsigned int, FMOD_TIMEUNIT, const char*, FMOD_SYNCPOINT**);
    errCheck(FMOD_Sound_AddSyncPoint(sound, offset, offsettype, name, &point));
    FMODExt_push_ptr_FMOD_SYNCPOINT(L, point);
    return 1;
}
#endif


/* FMOD_Sound_DeleteSyncPoint(input FMOD_SOUND* sound, input FMOD_SYNCPOINT* point, ) */
#ifndef FMODExt_func_FMOD_Sound_DeleteSyncPoint
#define FMODExt_func_FMOD_Sound_DeleteSyncPoint _FMODExt_func_FMOD_Sound_DeleteSyncPoint
static int _FMODExt_func_FMOD_Sound_DeleteSyncPoint(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_SYNCPOINT* point = FMODExt_check_ptr_FMOD_SYNCPOINT(L, 2);
    ensure(LL, FMOD_Sound_DeleteSyncPoint, FMOD_RESULT, FMOD_SOUND*, FMOD_SYNCPOINT*);
    errCheck(FMOD_Sound_DeleteSyncPoint(sound, point));
    return 0;
}
#endif


/* FMOD_Sound_SetMode(input FMOD_SOUND* sound, input FMOD_MODE mode, ) */
#ifndef FMODExt_func_FMOD_Sound_SetMode
#define FMODExt_func_FMOD_Sound_SetMode _FMODExt_func_FMOD_Sound_SetMode
static int _FMODExt_func_FMOD_Sound_SetMode(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_MODE mode = FMODExt_check_unsigned_int(L, 2);
    ensure(LL, FMOD_Sound_SetMode, FMOD_RESULT, FMOD_SOUND*, FMOD_MODE);
    errCheck(FMOD_Sound_SetMode(sound, mode));
    return 0;
}
#endif


/* FMOD_Sound_GetMode(input FMOD_SOUND* sound, output FMOD_MODE* mode, ) */
#ifndef FMODExt_func_FMOD_Sound_GetMode
#define FMODExt_func_FMOD_Sound_GetMode _FMODExt_func_FMOD_Sound_GetMode
static int _FMODExt_func_FMOD_Sound_GetMode(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    FMOD_MODE mode;
    ensure(LL, FMOD_Sound_GetMode, FMOD_RESULT, FMOD_SOUND*, FMOD_MODE*);
    errCheck(FMOD_Sound_GetMode(sound, &mode));
    FMODExt_push_unsigned_int(L, mode);
    return 1;
}
#endif


/* FMOD_Sound_SetLoopCount(input FMOD_SOUND* sound, input int loopcount, ) */
#ifndef FMODExt_func_FMOD_Sound_SetLoopCount
#define FMODExt_func_FMOD_Sound_SetLoopCount _FMODExt_func_FMOD_Sound_SetLoopCount
static int _FMODExt_func_FMOD_Sound_SetLoopCount(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int loopcount = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_Sound_SetLoopCount, FMOD_RESULT, FMOD_SOUND*, int);
    errCheck(FMOD_Sound_SetLoopCount(sound, loopcount));
    return 0;
}
#endif


/* FMOD_Sound_GetLoopCount(input FMOD_SOUND* sound, output int* loopcount, ) */
#ifndef FMODExt_func_FMOD_Sound_GetLoopCount
#define FMODExt_func_FMOD_Sound_GetLoopCount _FMODExt_func_FMOD_Sound_GetLoopCount
static int _FMODExt_func_FMOD_Sound_GetLoopCount(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int loopcount;
    ensure(LL, FMOD_Sound_GetLoopCount, FMOD_RESULT, FMOD_SOUND*, int*);
    errCheck(FMOD_Sound_GetLoopCount(sound, &loopcount));
    FMODExt_push_int(L, loopcount);
    return 1;
}
#endif


/* FMOD_Sound_SetLoopPoints(input FMOD_SOUND* sound, input unsigned int loopstart, input FMOD_TIMEUNIT loopstarttype, input unsigned int loopend, input FMOD_TIMEUNIT loopendtype, ) */
#ifndef FMODExt_func_FMOD_Sound_SetLoopPoints
#define FMODExt_func_FMOD_Sound_SetLoopPoints _FMODExt_func_FMOD_Sound_SetLoopPoints
static int _FMODExt_func_FMOD_Sound_SetLoopPoints(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    unsigned int loopstart = FMODExt_check_unsigned_int(L, 2);
    FMOD_TIMEUNIT loopstarttype = FMODExt_check_unsigned_int(L, 3);
    unsigned int loopend = FMODExt_check_unsigned_int(L, 4);
    FMOD_TIMEUNIT loopendtype = FMODExt_check_unsigned_int(L, 5);
    ensure(LL, FMOD_Sound_SetLoopPoints, FMOD_RESULT, FMOD_SOUND*, unsigned int, FMOD_TIMEUNIT, unsigned int, FMOD_TIMEUNIT);
    errCheck(FMOD_Sound_SetLoopPoints(sound, loopstart, loopstarttype, loopend, loopendtype));
    return 0;
}
#endif


/* FMOD_Sound_GetLoopPoints(input FMOD_SOUND* sound, output unsigned int* loopstart, input FMOD_TIMEUNIT loopstarttype, output unsigned int* loopend, input FMOD_TIMEUNIT loopendtype, ) */
#ifndef FMODExt_func_FMOD_Sound_GetLoopPoints
#define FMODExt_func_FMOD_Sound_GetLoopPoints _FMODExt_func_FMOD_Sound_GetLoopPoints
static int _FMODExt_func_FMOD_Sound_GetLoopPoints(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    unsigned int loopstart;
    FMOD_TIMEUNIT loopstarttype = FMODExt_check_unsigned_int(L, 2);
    unsigned int loopend;
    FMOD_TIMEUNIT loopendtype = FMODExt_check_unsigned_int(L, 3);
    ensure(LL, FMOD_Sound_GetLoopPoints, FMOD_RESULT, FMOD_SOUND*, unsigned int*, FMOD_TIMEUNIT, unsigned int*, FMOD_TIMEUNIT);
    errCheck(FMOD_Sound_GetLoopPoints(sound, &loopstart, loopstarttype, &loopend, loopendtype));
    FMODExt_push_unsigned_int(L, loopstart);
    FMODExt_push_unsigned_int(L, loopend);
    return 2;
}
#endif


/* FMOD_Sound_GetMusicNumChannels(input FMOD_SOUND* sound, output int* numchannels, ) */
#ifndef FMODExt_func_FMOD_Sound_GetMusicNumChannels
#define FMODExt_func_FMOD_Sound_GetMusicNumChannels _FMODExt_func_FMOD_Sound_GetMusicNumChannels
static int _FMODExt_func_FMOD_Sound_GetMusicNumChannels(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int numchannels;
    ensure(LL, FMOD_Sound_GetMusicNumChannels, FMOD_RESULT, FMOD_SOUND*, int*);
    errCheck(FMOD_Sound_GetMusicNumChannels(sound, &numchannels));
    FMODExt_push_int(L, numchannels);
    return 1;
}
#endif


/* FMOD_Sound_SetMusicChannelVolume(input FMOD_SOUND* sound, input int channel, input float volume, ) */
#ifndef FMODExt_func_FMOD_Sound_SetMusicChannelVolume
#define FMODExt_func_FMOD_Sound_SetMusicChannelVolume _FMODExt_func_FMOD_Sound_SetMusicChannelVolume
static int _FMODExt_func_FMOD_Sound_SetMusicChannelVolume(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int channel = FMODExt_check_int(L, 2);
    float volume = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_Sound_SetMusicChannelVolume, FMOD_RESULT, FMOD_SOUND*, int, float);
    errCheck(FMOD_Sound_SetMusicChannelVolume(sound, channel, volume));
    return 0;
}
#endif


/* FMOD_Sound_GetMusicChannelVolume(input FMOD_SOUND* sound, input int channel, output float* volume, ) */
#ifndef FMODExt_func_FMOD_Sound_GetMusicChannelVolume
#define FMODExt_func_FMOD_Sound_GetMusicChannelVolume _FMODExt_func_FMOD_Sound_GetMusicChannelVolume
static int _FMODExt_func_FMOD_Sound_GetMusicChannelVolume(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    int channel = FMODExt_check_int(L, 2);
    float volume;
    ensure(LL, FMOD_Sound_GetMusicChannelVolume, FMOD_RESULT, FMOD_SOUND*, int, float*);
    errCheck(FMOD_Sound_GetMusicChannelVolume(sound, channel, &volume));
    FMODExt_push_float(L, volume);
    return 1;
}
#endif


/* FMOD_Sound_SetMusicSpeed(input FMOD_SOUND* sound, input float speed, ) */
#ifndef FMODExt_func_FMOD_Sound_SetMusicSpeed
#define FMODExt_func_FMOD_Sound_SetMusicSpeed _FMODExt_func_FMOD_Sound_SetMusicSpeed
static int _FMODExt_func_FMOD_Sound_SetMusicSpeed(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    float speed = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_Sound_SetMusicSpeed, FMOD_RESULT, FMOD_SOUND*, float);
    errCheck(FMOD_Sound_SetMusicSpeed(sound, speed));
    return 0;
}
#endif


/* FMOD_Sound_GetMusicSpeed(input FMOD_SOUND* sound, output float* speed, ) */
#ifndef FMODExt_func_FMOD_Sound_GetMusicSpeed
#define FMODExt_func_FMOD_Sound_GetMusicSpeed _FMODExt_func_FMOD_Sound_GetMusicSpeed
static int _FMODExt_func_FMOD_Sound_GetMusicSpeed(lua_State *L) {
    FMOD_SOUND* sound = FMODExt_check_ptr_FMOD_SOUND(L, 1);
    float speed;
    ensure(LL, FMOD_Sound_GetMusicSpeed, FMOD_RESULT, FMOD_SOUND*, float*);
    errCheck(FMOD_Sound_GetMusicSpeed(sound, &speed));
    FMODExt_push_float(L, speed);
    return 1;
}
#endif


/* FMOD_Sound_SetUserData(input FMOD_SOUND* sound, unknown void* userdata, ) */


/* FMOD_Sound_GetUserData(input FMOD_SOUND* sound, unknown void** userdata, ) */


/* FMOD_Channel_GetSystemObject(input FMOD_CHANNEL* channel, output FMOD_SYSTEM** system, ) */
#ifndef FMODExt_func_FMOD_Channel_GetSystemObject
#define FMODExt_func_FMOD_Channel_GetSystemObject _FMODExt_func_FMOD_Channel_GetSystemObject
static int _FMODExt_func_FMOD_Channel_GetSystemObject(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_SYSTEM* system;
    ensure(LL, FMOD_Channel_GetSystemObject, FMOD_RESULT, FMOD_CHANNEL*, FMOD_SYSTEM**);
    errCheck(FMOD_Channel_GetSystemObject(channel, &system));
    FMODExt_push_ptr_FMOD_SYSTEM(L, system);
    return 1;
}
#endif


/* FMOD_Channel_Stop(input FMOD_CHANNEL* channel, ) */
#ifndef FMODExt_func_FMOD_Channel_Stop
#define FMODExt_func_FMOD_Channel_Stop _FMODExt_func_FMOD_Channel_Stop
static int _FMODExt_func_FMOD_Channel_Stop(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    ensure(LL, FMOD_Channel_Stop, FMOD_RESULT, FMOD_CHANNEL*);
    errCheck(FMOD_Channel_Stop(channel));
    return 0;
}
#endif


/* FMOD_Channel_SetPaused(input FMOD_CHANNEL* channel, input FMOD_BOOL paused, ) */
#ifndef FMODExt_func_FMOD_Channel_SetPaused
#define FMODExt_func_FMOD_Channel_SetPaused _FMODExt_func_FMOD_Channel_SetPaused
static int _FMODExt_func_FMOD_Channel_SetPaused(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL paused = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_Channel_SetPaused, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL);
    errCheck(FMOD_Channel_SetPaused(channel, paused));
    return 0;
}
#endif


/* FMOD_Channel_GetPaused(input FMOD_CHANNEL* channel, output FMOD_BOOL* paused, ) */
#ifndef FMODExt_func_FMOD_Channel_GetPaused
#define FMODExt_func_FMOD_Channel_GetPaused _FMODExt_func_FMOD_Channel_GetPaused
static int _FMODExt_func_FMOD_Channel_GetPaused(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL paused;
    ensure(LL, FMOD_Channel_GetPaused, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL*);
    errCheck(FMOD_Channel_GetPaused(channel, &paused));
    FMODExt_push_FMOD_BOOL(L, paused);
    return 1;
}
#endif


/* FMOD_Channel_SetVolume(input FMOD_CHANNEL* channel, input float volume, ) */
#ifndef FMODExt_func_FMOD_Channel_SetVolume
#define FMODExt_func_FMOD_Channel_SetVolume _FMODExt_func_FMOD_Channel_SetVolume
static int _FMODExt_func_FMOD_Channel_SetVolume(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float volume = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_Channel_SetVolume, FMOD_RESULT, FMOD_CHANNEL*, float);
    errCheck(FMOD_Channel_SetVolume(channel, volume));
    return 0;
}
#endif


/* FMOD_Channel_GetVolume(input FMOD_CHANNEL* channel, output float* volume, ) */
#ifndef FMODExt_func_FMOD_Channel_GetVolume
#define FMODExt_func_FMOD_Channel_GetVolume _FMODExt_func_FMOD_Channel_GetVolume
static int _FMODExt_func_FMOD_Channel_GetVolume(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float volume;
    ensure(LL, FMOD_Channel_GetVolume, FMOD_RESULT, FMOD_CHANNEL*, float*);
    errCheck(FMOD_Channel_GetVolume(channel, &volume));
    FMODExt_push_float(L, volume);
    return 1;
}
#endif


/* FMOD_Channel_SetVolumeRamp(input FMOD_CHANNEL* channel, input FMOD_BOOL ramp, ) */
#ifndef FMODExt_func_FMOD_Channel_SetVolumeRamp
#define FMODExt_func_FMOD_Channel_SetVolumeRamp _FMODExt_func_FMOD_Channel_SetVolumeRamp
static int _FMODExt_func_FMOD_Channel_SetVolumeRamp(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL ramp = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_Channel_SetVolumeRamp, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL);
    errCheck(FMOD_Channel_SetVolumeRamp(channel, ramp));
    return 0;
}
#endif


/* FMOD_Channel_GetVolumeRamp(input FMOD_CHANNEL* channel, output FMOD_BOOL* ramp, ) */
#ifndef FMODExt_func_FMOD_Channel_GetVolumeRamp
#define FMODExt_func_FMOD_Channel_GetVolumeRamp _FMODExt_func_FMOD_Channel_GetVolumeRamp
static int _FMODExt_func_FMOD_Channel_GetVolumeRamp(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL ramp;
    ensure(LL, FMOD_Channel_GetVolumeRamp, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL*);
    errCheck(FMOD_Channel_GetVolumeRamp(channel, &ramp));
    FMODExt_push_FMOD_BOOL(L, ramp);
    return 1;
}
#endif


/* FMOD_Channel_GetAudibility(input FMOD_CHANNEL* channel, output float* audibility, ) */
#ifndef FMODExt_func_FMOD_Channel_GetAudibility
#define FMODExt_func_FMOD_Channel_GetAudibility _FMODExt_func_FMOD_Channel_GetAudibility
static int _FMODExt_func_FMOD_Channel_GetAudibility(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float audibility;
    ensure(LL, FMOD_Channel_GetAudibility, FMOD_RESULT, FMOD_CHANNEL*, float*);
    errCheck(FMOD_Channel_GetAudibility(channel, &audibility));
    FMODExt_push_float(L, audibility);
    return 1;
}
#endif


/* FMOD_Channel_SetPitch(input FMOD_CHANNEL* channel, input float pitch, ) */
#ifndef FMODExt_func_FMOD_Channel_SetPitch
#define FMODExt_func_FMOD_Channel_SetPitch _FMODExt_func_FMOD_Channel_SetPitch
static int _FMODExt_func_FMOD_Channel_SetPitch(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float pitch = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_Channel_SetPitch, FMOD_RESULT, FMOD_CHANNEL*, float);
    errCheck(FMOD_Channel_SetPitch(channel, pitch));
    return 0;
}
#endif


/* FMOD_Channel_GetPitch(input FMOD_CHANNEL* channel, output float* pitch, ) */
#ifndef FMODExt_func_FMOD_Channel_GetPitch
#define FMODExt_func_FMOD_Channel_GetPitch _FMODExt_func_FMOD_Channel_GetPitch
static int _FMODExt_func_FMOD_Channel_GetPitch(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float pitch;
    ensure(LL, FMOD_Channel_GetPitch, FMOD_RESULT, FMOD_CHANNEL*, float*);
    errCheck(FMOD_Channel_GetPitch(channel, &pitch));
    FMODExt_push_float(L, pitch);
    return 1;
}
#endif


/* FMOD_Channel_SetMute(input FMOD_CHANNEL* channel, input FMOD_BOOL mute, ) */
#ifndef FMODExt_func_FMOD_Channel_SetMute
#define FMODExt_func_FMOD_Channel_SetMute _FMODExt_func_FMOD_Channel_SetMute
static int _FMODExt_func_FMOD_Channel_SetMute(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL mute = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_Channel_SetMute, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL);
    errCheck(FMOD_Channel_SetMute(channel, mute));
    return 0;
}
#endif


/* FMOD_Channel_GetMute(input FMOD_CHANNEL* channel, output FMOD_BOOL* mute, ) */
#ifndef FMODExt_func_FMOD_Channel_GetMute
#define FMODExt_func_FMOD_Channel_GetMute _FMODExt_func_FMOD_Channel_GetMute
static int _FMODExt_func_FMOD_Channel_GetMute(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL mute;
    ensure(LL, FMOD_Channel_GetMute, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL*);
    errCheck(FMOD_Channel_GetMute(channel, &mute));
    FMODExt_push_FMOD_BOOL(L, mute);
    return 1;
}
#endif


/* FMOD_Channel_SetReverbProperties(input FMOD_CHANNEL* channel, input int instance, input float wet, ) */
#ifndef FMODExt_func_FMOD_Channel_SetReverbProperties
#define FMODExt_func_FMOD_Channel_SetReverbProperties _FMODExt_func_FMOD_Channel_SetReverbProperties
static int _FMODExt_func_FMOD_Channel_SetReverbProperties(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int instance = FMODExt_check_int(L, 2);
    float wet = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_Channel_SetReverbProperties, FMOD_RESULT, FMOD_CHANNEL*, int, float);
    errCheck(FMOD_Channel_SetReverbProperties(channel, instance, wet));
    return 0;
}
#endif


/* FMOD_Channel_GetReverbProperties(input FMOD_CHANNEL* channel, input int instance, output float* wet, ) */
#ifndef FMODExt_func_FMOD_Channel_GetReverbProperties
#define FMODExt_func_FMOD_Channel_GetReverbProperties _FMODExt_func_FMOD_Channel_GetReverbProperties
static int _FMODExt_func_FMOD_Channel_GetReverbProperties(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int instance = FMODExt_check_int(L, 2);
    float wet;
    ensure(LL, FMOD_Channel_GetReverbProperties, FMOD_RESULT, FMOD_CHANNEL*, int, float*);
    errCheck(FMOD_Channel_GetReverbProperties(channel, instance, &wet));
    FMODExt_push_float(L, wet);
    return 1;
}
#endif


/* FMOD_Channel_SetLowPassGain(input FMOD_CHANNEL* channel, input float gain, ) */
#ifndef FMODExt_func_FMOD_Channel_SetLowPassGain
#define FMODExt_func_FMOD_Channel_SetLowPassGain _FMODExt_func_FMOD_Channel_SetLowPassGain
static int _FMODExt_func_FMOD_Channel_SetLowPassGain(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float gain = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_Channel_SetLowPassGain, FMOD_RESULT, FMOD_CHANNEL*, float);
    errCheck(FMOD_Channel_SetLowPassGain(channel, gain));
    return 0;
}
#endif


/* FMOD_Channel_GetLowPassGain(input FMOD_CHANNEL* channel, output float* gain, ) */
#ifndef FMODExt_func_FMOD_Channel_GetLowPassGain
#define FMODExt_func_FMOD_Channel_GetLowPassGain _FMODExt_func_FMOD_Channel_GetLowPassGain
static int _FMODExt_func_FMOD_Channel_GetLowPassGain(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float gain;
    ensure(LL, FMOD_Channel_GetLowPassGain, FMOD_RESULT, FMOD_CHANNEL*, float*);
    errCheck(FMOD_Channel_GetLowPassGain(channel, &gain));
    FMODExt_push_float(L, gain);
    return 1;
}
#endif


/* FMOD_Channel_SetMode(input FMOD_CHANNEL* channel, input FMOD_MODE mode, ) */
#ifndef FMODExt_func_FMOD_Channel_SetMode
#define FMODExt_func_FMOD_Channel_SetMode _FMODExt_func_FMOD_Channel_SetMode
static int _FMODExt_func_FMOD_Channel_SetMode(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_MODE mode = FMODExt_check_unsigned_int(L, 2);
    ensure(LL, FMOD_Channel_SetMode, FMOD_RESULT, FMOD_CHANNEL*, FMOD_MODE);
    errCheck(FMOD_Channel_SetMode(channel, mode));
    return 0;
}
#endif


/* FMOD_Channel_GetMode(input FMOD_CHANNEL* channel, output FMOD_MODE* mode, ) */
#ifndef FMODExt_func_FMOD_Channel_GetMode
#define FMODExt_func_FMOD_Channel_GetMode _FMODExt_func_FMOD_Channel_GetMode
static int _FMODExt_func_FMOD_Channel_GetMode(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_MODE mode;
    ensure(LL, FMOD_Channel_GetMode, FMOD_RESULT, FMOD_CHANNEL*, FMOD_MODE*);
    errCheck(FMOD_Channel_GetMode(channel, &mode));
    FMODExt_push_unsigned_int(L, mode);
    return 1;
}
#endif


/* FMOD_Channel_SetCallback(input FMOD_CHANNEL* channel, unknown FMOD_CHANNELCONTROL_CALLBACK callback, ) */


/* FMOD_Channel_IsPlaying(input FMOD_CHANNEL* channel, output FMOD_BOOL* isplaying, ) */
#ifndef FMODExt_func_FMOD_Channel_IsPlaying
#define FMODExt_func_FMOD_Channel_IsPlaying _FMODExt_func_FMOD_Channel_IsPlaying
static int _FMODExt_func_FMOD_Channel_IsPlaying(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL isplaying;
    ensure(LL, FMOD_Channel_IsPlaying, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL*);
    errCheck(FMOD_Channel_IsPlaying(channel, &isplaying));
    FMODExt_push_FMOD_BOOL(L, isplaying);
    return 1;
}
#endif


/* FMOD_Channel_SetPan(input FMOD_CHANNEL* channel, input float pan, ) */
#ifndef FMODExt_func_FMOD_Channel_SetPan
#define FMODExt_func_FMOD_Channel_SetPan _FMODExt_func_FMOD_Channel_SetPan
static int _FMODExt_func_FMOD_Channel_SetPan(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float pan = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_Channel_SetPan, FMOD_RESULT, FMOD_CHANNEL*, float);
    errCheck(FMOD_Channel_SetPan(channel, pan));
    return 0;
}
#endif


/* FMOD_Channel_SetMixLevelsOutput(input FMOD_CHANNEL* channel, input float frontleft, input float frontright, input float center, input float lfe, input float surroundleft, input float surroundright, input float backleft, input float backright, ) */
#ifndef FMODExt_func_FMOD_Channel_SetMixLevelsOutput
#define FMODExt_func_FMOD_Channel_SetMixLevelsOutput _FMODExt_func_FMOD_Channel_SetMixLevelsOutput
static int _FMODExt_func_FMOD_Channel_SetMixLevelsOutput(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float frontleft = FMODExt_check_float(L, 2);
    float frontright = FMODExt_check_float(L, 3);
    float center = FMODExt_check_float(L, 4);
    float lfe = FMODExt_check_float(L, 5);
    float surroundleft = FMODExt_check_float(L, 6);
    float surroundright = FMODExt_check_float(L, 7);
    float backleft = FMODExt_check_float(L, 8);
    float backright = FMODExt_check_float(L, 9);
    ensure(LL, FMOD_Channel_SetMixLevelsOutput, FMOD_RESULT, FMOD_CHANNEL*, float, float, float, float, float, float, float, float);
    errCheck(FMOD_Channel_SetMixLevelsOutput(channel, frontleft, frontright, center, lfe, surroundleft, surroundright, backleft, backright));
    return 0;
}
#endif


/* FMOD_Channel_SetMixLevelsInput(input FMOD_CHANNEL* channel, output float* levels, input int numlevels, ) */
#ifndef FMODExt_func_FMOD_Channel_SetMixLevelsInput
#define FMODExt_func_FMOD_Channel_SetMixLevelsInput _FMODExt_func_FMOD_Channel_SetMixLevelsInput
static int _FMODExt_func_FMOD_Channel_SetMixLevelsInput(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float levels;
    int numlevels = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_Channel_SetMixLevelsInput, FMOD_RESULT, FMOD_CHANNEL*, float*, int);
    errCheck(FMOD_Channel_SetMixLevelsInput(channel, &levels, numlevels));
    FMODExt_push_float(L, levels);
    return 1;
}
#endif


/* FMOD_Channel_SetMixMatrix(input FMOD_CHANNEL* channel, output float* matrix, input int outchannels, input int inchannels, input int inchannel_hop, ) */
#ifndef FMODExt_func_FMOD_Channel_SetMixMatrix
#define FMODExt_func_FMOD_Channel_SetMixMatrix _FMODExt_func_FMOD_Channel_SetMixMatrix
static int _FMODExt_func_FMOD_Channel_SetMixMatrix(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float matrix;
    int outchannels = FMODExt_check_int(L, 2);
    int inchannels = FMODExt_check_int(L, 3);
    int inchannel_hop = FMODExt_check_int(L, 4);
    ensure(LL, FMOD_Channel_SetMixMatrix, FMOD_RESULT, FMOD_CHANNEL*, float*, int, int, int);
    errCheck(FMOD_Channel_SetMixMatrix(channel, &matrix, outchannels, inchannels, inchannel_hop));
    FMODExt_push_float(L, matrix);
    return 1;
}
#endif


/* FMOD_Channel_GetMixMatrix(input FMOD_CHANNEL* channel, output float* matrix, output int* outchannels, output int* inchannels, input int inchannel_hop, ) */
#ifndef FMODExt_func_FMOD_Channel_GetMixMatrix
#define FMODExt_func_FMOD_Channel_GetMixMatrix _FMODExt_func_FMOD_Channel_GetMixMatrix
static int _FMODExt_func_FMOD_Channel_GetMixMatrix(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float matrix;
    int outchannels;
    int inchannels;
    int inchannel_hop = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_Channel_GetMixMatrix, FMOD_RESULT, FMOD_CHANNEL*, float*, int*, int*, int);
    errCheck(FMOD_Channel_GetMixMatrix(channel, &matrix, &outchannels, &inchannels, inchannel_hop));
    FMODExt_push_float(L, matrix);
    FMODExt_push_int(L, outchannels);
    FMODExt_push_int(L, inchannels);
    return 3;
}
#endif


/* FMOD_Channel_GetDSPClock(input FMOD_CHANNEL* channel, output unsigned long long* dspclock, output unsigned long long* parentclock, ) */
#ifndef FMODExt_func_FMOD_Channel_GetDSPClock
#define FMODExt_func_FMOD_Channel_GetDSPClock _FMODExt_func_FMOD_Channel_GetDSPClock
static int _FMODExt_func_FMOD_Channel_GetDSPClock(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned long long dspclock;
    unsigned long long parentclock;
    ensure(LL, FMOD_Channel_GetDSPClock, FMOD_RESULT, FMOD_CHANNEL*, unsigned long long*, unsigned long long*);
    errCheck(FMOD_Channel_GetDSPClock(channel, &dspclock, &parentclock));
    FMODExt_push_unsigned_long_long(L, dspclock);
    FMODExt_push_unsigned_long_long(L, parentclock);
    return 2;
}
#endif


/* FMOD_Channel_SetDelay(input FMOD_CHANNEL* channel, input unsigned long long dspclock_start, input unsigned long long dspclock_end, input FMOD_BOOL stopchannels, ) */
#ifndef FMODExt_func_FMOD_Channel_SetDelay
#define FMODExt_func_FMOD_Channel_SetDelay _FMODExt_func_FMOD_Channel_SetDelay
static int _FMODExt_func_FMOD_Channel_SetDelay(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned long long dspclock_start = FMODExt_check_unsigned_long_long(L, 2);
    unsigned long long dspclock_end = FMODExt_check_unsigned_long_long(L, 3);
    FMOD_BOOL stopchannels = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(LL, FMOD_Channel_SetDelay, FMOD_RESULT, FMOD_CHANNEL*, unsigned long long, unsigned long long, FMOD_BOOL);
    errCheck(FMOD_Channel_SetDelay(channel, dspclock_start, dspclock_end, stopchannels));
    return 0;
}
#endif


/* FMOD_Channel_GetDelay(input FMOD_CHANNEL* channel, output unsigned long long* dspclock_start, output unsigned long long* dspclock_end, output FMOD_BOOL* stopchannels, ) */
#ifndef FMODExt_func_FMOD_Channel_GetDelay
#define FMODExt_func_FMOD_Channel_GetDelay _FMODExt_func_FMOD_Channel_GetDelay
static int _FMODExt_func_FMOD_Channel_GetDelay(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned long long dspclock_start;
    unsigned long long dspclock_end;
    FMOD_BOOL stopchannels;
    ensure(LL, FMOD_Channel_GetDelay, FMOD_RESULT, FMOD_CHANNEL*, unsigned long long*, unsigned long long*, FMOD_BOOL*);
    errCheck(FMOD_Channel_GetDelay(channel, &dspclock_start, &dspclock_end, &stopchannels));
    FMODExt_push_unsigned_long_long(L, dspclock_start);
    FMODExt_push_unsigned_long_long(L, dspclock_end);
    FMODExt_push_FMOD_BOOL(L, stopchannels);
    return 3;
}
#endif


/* FMOD_Channel_AddFadePoint(input FMOD_CHANNEL* channel, input unsigned long long dspclock, input float volume, ) */
#ifndef FMODExt_func_FMOD_Channel_AddFadePoint
#define FMODExt_func_FMOD_Channel_AddFadePoint _FMODExt_func_FMOD_Channel_AddFadePoint
static int _FMODExt_func_FMOD_Channel_AddFadePoint(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned long long dspclock = FMODExt_check_unsigned_long_long(L, 2);
    float volume = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_Channel_AddFadePoint, FMOD_RESULT, FMOD_CHANNEL*, unsigned long long, float);
    errCheck(FMOD_Channel_AddFadePoint(channel, dspclock, volume));
    return 0;
}
#endif


/* FMOD_Channel_SetFadePointRamp(input FMOD_CHANNEL* channel, input unsigned long long dspclock, input float volume, ) */
#ifndef FMODExt_func_FMOD_Channel_SetFadePointRamp
#define FMODExt_func_FMOD_Channel_SetFadePointRamp _FMODExt_func_FMOD_Channel_SetFadePointRamp
static int _FMODExt_func_FMOD_Channel_SetFadePointRamp(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned long long dspclock = FMODExt_check_unsigned_long_long(L, 2);
    float volume = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_Channel_SetFadePointRamp, FMOD_RESULT, FMOD_CHANNEL*, unsigned long long, float);
    errCheck(FMOD_Channel_SetFadePointRamp(channel, dspclock, volume));
    return 0;
}
#endif


/* FMOD_Channel_RemoveFadePoints(input FMOD_CHANNEL* channel, input unsigned long long dspclock_start, input unsigned long long dspclock_end, ) */
#ifndef FMODExt_func_FMOD_Channel_RemoveFadePoints
#define FMODExt_func_FMOD_Channel_RemoveFadePoints _FMODExt_func_FMOD_Channel_RemoveFadePoints
static int _FMODExt_func_FMOD_Channel_RemoveFadePoints(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned long long dspclock_start = FMODExt_check_unsigned_long_long(L, 2);
    unsigned long long dspclock_end = FMODExt_check_unsigned_long_long(L, 3);
    ensure(LL, FMOD_Channel_RemoveFadePoints, FMOD_RESULT, FMOD_CHANNEL*, unsigned long long, unsigned long long);
    errCheck(FMOD_Channel_RemoveFadePoints(channel, dspclock_start, dspclock_end));
    return 0;
}
#endif


/* FMOD_Channel_GetFadePoints(input FMOD_CHANNEL* channel, output unsigned int* numpoints, output unsigned long long* point_dspclock, output float* point_volume, ) */
#ifndef FMODExt_func_FMOD_Channel_GetFadePoints
#define FMODExt_func_FMOD_Channel_GetFadePoints _FMODExt_func_FMOD_Channel_GetFadePoints
static int _FMODExt_func_FMOD_Channel_GetFadePoints(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned int numpoints;
    unsigned long long point_dspclock;
    float point_volume;
    ensure(LL, FMOD_Channel_GetFadePoints, FMOD_RESULT, FMOD_CHANNEL*, unsigned int*, unsigned long long*, float*);
    errCheck(FMOD_Channel_GetFadePoints(channel, &numpoints, &point_dspclock, &point_volume));
    FMODExt_push_unsigned_int(L, numpoints);
    FMODExt_push_unsigned_long_long(L, point_dspclock);
    FMODExt_push_float(L, point_volume);
    return 3;
}
#endif


/* FMOD_Channel_GetDSP(input FMOD_CHANNEL* channel, input int index, output FMOD_DSP** dsp, ) */
#ifndef FMODExt_func_FMOD_Channel_GetDSP
#define FMODExt_func_FMOD_Channel_GetDSP _FMODExt_func_FMOD_Channel_GetDSP
static int _FMODExt_func_FMOD_Channel_GetDSP(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_DSP* dsp;
    ensure(LL, FMOD_Channel_GetDSP, FMOD_RESULT, FMOD_CHANNEL*, int, FMOD_DSP**);
    errCheck(FMOD_Channel_GetDSP(channel, index, &dsp));
    FMODExt_push_ptr_FMOD_DSP(L, dsp);
    return 1;
}
#endif


/* FMOD_Channel_AddDSP(input FMOD_CHANNEL* channel, input int index, input FMOD_DSP* dsp, ) */
#ifndef FMODExt_func_FMOD_Channel_AddDSP
#define FMODExt_func_FMOD_Channel_AddDSP _FMODExt_func_FMOD_Channel_AddDSP
static int _FMODExt_func_FMOD_Channel_AddDSP(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 3);
    ensure(LL, FMOD_Channel_AddDSP, FMOD_RESULT, FMOD_CHANNEL*, int, FMOD_DSP*);
    errCheck(FMOD_Channel_AddDSP(channel, index, dsp));
    return 0;
}
#endif


/* FMOD_Channel_RemoveDSP(input FMOD_CHANNEL* channel, input FMOD_DSP* dsp, ) */
#ifndef FMODExt_func_FMOD_Channel_RemoveDSP
#define FMODExt_func_FMOD_Channel_RemoveDSP _FMODExt_func_FMOD_Channel_RemoveDSP
static int _FMODExt_func_FMOD_Channel_RemoveDSP(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 2);
    ensure(LL, FMOD_Channel_RemoveDSP, FMOD_RESULT, FMOD_CHANNEL*, FMOD_DSP*);
    errCheck(FMOD_Channel_RemoveDSP(channel, dsp));
    return 0;
}
#endif


/* FMOD_Channel_GetNumDSPs(input FMOD_CHANNEL* channel, output int* numdsps, ) */
#ifndef FMODExt_func_FMOD_Channel_GetNumDSPs
#define FMODExt_func_FMOD_Channel_GetNumDSPs _FMODExt_func_FMOD_Channel_GetNumDSPs
static int _FMODExt_func_FMOD_Channel_GetNumDSPs(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int numdsps;
    ensure(LL, FMOD_Channel_GetNumDSPs, FMOD_RESULT, FMOD_CHANNEL*, int*);
    errCheck(FMOD_Channel_GetNumDSPs(channel, &numdsps));
    FMODExt_push_int(L, numdsps);
    return 1;
}
#endif


/* FMOD_Channel_SetDSPIndex(input FMOD_CHANNEL* channel, input FMOD_DSP* dsp, input int index, ) */
#ifndef FMODExt_func_FMOD_Channel_SetDSPIndex
#define FMODExt_func_FMOD_Channel_SetDSPIndex _FMODExt_func_FMOD_Channel_SetDSPIndex
static int _FMODExt_func_FMOD_Channel_SetDSPIndex(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 2);
    int index = FMODExt_check_int(L, 3);
    ensure(LL, FMOD_Channel_SetDSPIndex, FMOD_RESULT, FMOD_CHANNEL*, FMOD_DSP*, int);
    errCheck(FMOD_Channel_SetDSPIndex(channel, dsp, index));
    return 0;
}
#endif


/* FMOD_Channel_GetDSPIndex(input FMOD_CHANNEL* channel, input FMOD_DSP* dsp, output int* index, ) */
#ifndef FMODExt_func_FMOD_Channel_GetDSPIndex
#define FMODExt_func_FMOD_Channel_GetDSPIndex _FMODExt_func_FMOD_Channel_GetDSPIndex
static int _FMODExt_func_FMOD_Channel_GetDSPIndex(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 2);
    int index;
    ensure(LL, FMOD_Channel_GetDSPIndex, FMOD_RESULT, FMOD_CHANNEL*, FMOD_DSP*, int*);
    errCheck(FMOD_Channel_GetDSPIndex(channel, dsp, &index));
    FMODExt_push_int(L, index);
    return 1;
}
#endif


/* FMOD_Channel_Set3DAttributes(input FMOD_CHANNEL* channel, input_ptr const FMOD_VECTOR* pos, input_ptr const FMOD_VECTOR* vel, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DAttributes
#define FMODExt_func_FMOD_Channel_Set3DAttributes _FMODExt_func_FMOD_Channel_Set3DAttributes
static int _FMODExt_func_FMOD_Channel_Set3DAttributes(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    const FMOD_VECTOR pos = FMODExt_check_FMOD_VECTOR(L, 2);
    const FMOD_VECTOR vel = FMODExt_check_FMOD_VECTOR(L, 3);
    ensure(LL, FMOD_Channel_Set3DAttributes, FMOD_RESULT, FMOD_CHANNEL*, const FMOD_VECTOR*, const FMOD_VECTOR*);
    errCheck(FMOD_Channel_Set3DAttributes(channel, &pos, &vel));
    return 0;
}
#endif


/* FMOD_Channel_Get3DAttributes(input FMOD_CHANNEL* channel, output FMOD_VECTOR* pos, output FMOD_VECTOR* vel, ) */
#ifndef FMODExt_func_FMOD_Channel_Get3DAttributes
#define FMODExt_func_FMOD_Channel_Get3DAttributes _FMODExt_func_FMOD_Channel_Get3DAttributes
static int _FMODExt_func_FMOD_Channel_Get3DAttributes(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_VECTOR pos;
    FMOD_VECTOR vel;
    ensure(LL, FMOD_Channel_Get3DAttributes, FMOD_RESULT, FMOD_CHANNEL*, FMOD_VECTOR*, FMOD_VECTOR*);
    errCheck(FMOD_Channel_Get3DAttributes(channel, &pos, &vel));
    FMODExt_push_FMOD_VECTOR(L, pos);
    FMODExt_push_FMOD_VECTOR(L, vel);
    return 2;
}
#endif


/* FMOD_Channel_Set3DMinMaxDistance(input FMOD_CHANNEL* channel, input float mindistance, input float maxdistance, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DMinMaxDistance
#define FMODExt_func_FMOD_Channel_Set3DMinMaxDistance _FMODExt_func_FMOD_Channel_Set3DMinMaxDistance
static int _FMODExt_func_FMOD_Channel_Set3DMinMaxDistance(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float mindistance = FMODExt_check_float(L, 2);
    float maxdistance = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_Channel_Set3DMinMaxDistance, FMOD_RESULT, FMOD_CHANNEL*, float, float);
    errCheck(FMOD_Channel_Set3DMinMaxDistance(channel, mindistance, maxdistance));
    return 0;
}
#endif


/* FMOD_Channel_Get3DMinMaxDistance(input FMOD_CHANNEL* channel, output float* mindistance, output float* maxdistance, ) */
#ifndef FMODExt_func_FMOD_Channel_Get3DMinMaxDistance
#define FMODExt_func_FMOD_Channel_Get3DMinMaxDistance _FMODExt_func_FMOD_Channel_Get3DMinMaxDistance
static int _FMODExt_func_FMOD_Channel_Get3DMinMaxDistance(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float mindistance;
    float maxdistance;
    ensure(LL, FMOD_Channel_Get3DMinMaxDistance, FMOD_RESULT, FMOD_CHANNEL*, float*, float*);
    errCheck(FMOD_Channel_Get3DMinMaxDistance(channel, &mindistance, &maxdistance));
    FMODExt_push_float(L, mindistance);
    FMODExt_push_float(L, maxdistance);
    return 2;
}
#endif


/* FMOD_Channel_Set3DConeSettings(input FMOD_CHANNEL* channel, input float insideconeangle, input float outsideconeangle, input float outsidevolume, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DConeSettings
#define FMODExt_func_FMOD_Channel_Set3DConeSettings _FMODExt_func_FMOD_Channel_Set3DConeSettings
static int _FMODExt_func_FMOD_Channel_Set3DConeSettings(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float insideconeangle = FMODExt_check_float(L, 2);
    float outsideconeangle = FMODExt_check_float(L, 3);
    float outsidevolume = FMODExt_check_float(L, 4);
    ensure(LL, FMOD_Channel_Set3DConeSettings, FMOD_RESULT, FMOD_CHANNEL*, float, float, float);
    errCheck(FMOD_Channel_Set3DConeSettings(channel, insideconeangle, outsideconeangle, outsidevolume));
    return 0;
}
#endif


/* FMOD_Channel_Get3DConeSettings(input FMOD_CHANNEL* channel, output float* insideconeangle, output float* outsideconeangle, output float* outsidevolume, ) */
#ifndef FMODExt_func_FMOD_Channel_Get3DConeSettings
#define FMODExt_func_FMOD_Channel_Get3DConeSettings _FMODExt_func_FMOD_Channel_Get3DConeSettings
static int _FMODExt_func_FMOD_Channel_Get3DConeSettings(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float insideconeangle;
    float outsideconeangle;
    float outsidevolume;
    ensure(LL, FMOD_Channel_Get3DConeSettings, FMOD_RESULT, FMOD_CHANNEL*, float*, float*, float*);
    errCheck(FMOD_Channel_Get3DConeSettings(channel, &insideconeangle, &outsideconeangle, &outsidevolume));
    FMODExt_push_float(L, insideconeangle);
    FMODExt_push_float(L, outsideconeangle);
    FMODExt_push_float(L, outsidevolume);
    return 3;
}
#endif


/* FMOD_Channel_Set3DConeOrientation(input FMOD_CHANNEL* channel, output FMOD_VECTOR* orientation, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DConeOrientation
#define FMODExt_func_FMOD_Channel_Set3DConeOrientation _FMODExt_func_FMOD_Channel_Set3DConeOrientation
static int _FMODExt_func_FMOD_Channel_Set3DConeOrientation(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_VECTOR orientation;
    ensure(LL, FMOD_Channel_Set3DConeOrientation, FMOD_RESULT, FMOD_CHANNEL*, FMOD_VECTOR*);
    errCheck(FMOD_Channel_Set3DConeOrientation(channel, &orientation));
    FMODExt_push_FMOD_VECTOR(L, orientation);
    return 1;
}
#endif


/* FMOD_Channel_Get3DConeOrientation(input FMOD_CHANNEL* channel, output FMOD_VECTOR* orientation, ) */
#ifndef FMODExt_func_FMOD_Channel_Get3DConeOrientation
#define FMODExt_func_FMOD_Channel_Get3DConeOrientation _FMODExt_func_FMOD_Channel_Get3DConeOrientation
static int _FMODExt_func_FMOD_Channel_Get3DConeOrientation(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_VECTOR orientation;
    ensure(LL, FMOD_Channel_Get3DConeOrientation, FMOD_RESULT, FMOD_CHANNEL*, FMOD_VECTOR*);
    errCheck(FMOD_Channel_Get3DConeOrientation(channel, &orientation));
    FMODExt_push_FMOD_VECTOR(L, orientation);
    return 1;
}
#endif


/* FMOD_Channel_Set3DCustomRolloff(input FMOD_CHANNEL* channel, output FMOD_VECTOR* points, input int numpoints, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DCustomRolloff
#define FMODExt_func_FMOD_Channel_Set3DCustomRolloff _FMODExt_func_FMOD_Channel_Set3DCustomRolloff
static int _FMODExt_func_FMOD_Channel_Set3DCustomRolloff(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_VECTOR points;
    int numpoints = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_Channel_Set3DCustomRolloff, FMOD_RESULT, FMOD_CHANNEL*, FMOD_VECTOR*, int);
    errCheck(FMOD_Channel_Set3DCustomRolloff(channel, &points, numpoints));
    FMODExt_push_FMOD_VECTOR(L, points);
    return 1;
}
#endif


/* FMOD_Channel_Get3DCustomRolloff(input FMOD_CHANNEL* channel, unknown FMOD_VECTOR** points, output int* numpoints, ) */


/* FMOD_Channel_Set3DOcclusion(input FMOD_CHANNEL* channel, input float directocclusion, input float reverbocclusion, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DOcclusion
#define FMODExt_func_FMOD_Channel_Set3DOcclusion _FMODExt_func_FMOD_Channel_Set3DOcclusion
static int _FMODExt_func_FMOD_Channel_Set3DOcclusion(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float directocclusion = FMODExt_check_float(L, 2);
    float reverbocclusion = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_Channel_Set3DOcclusion, FMOD_RESULT, FMOD_CHANNEL*, float, float);
    errCheck(FMOD_Channel_Set3DOcclusion(channel, directocclusion, reverbocclusion));
    return 0;
}
#endif


/* FMOD_Channel_Get3DOcclusion(input FMOD_CHANNEL* channel, output float* directocclusion, output float* reverbocclusion, ) */
#ifndef FMODExt_func_FMOD_Channel_Get3DOcclusion
#define FMODExt_func_FMOD_Channel_Get3DOcclusion _FMODExt_func_FMOD_Channel_Get3DOcclusion
static int _FMODExt_func_FMOD_Channel_Get3DOcclusion(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float directocclusion;
    float reverbocclusion;
    ensure(LL, FMOD_Channel_Get3DOcclusion, FMOD_RESULT, FMOD_CHANNEL*, float*, float*);
    errCheck(FMOD_Channel_Get3DOcclusion(channel, &directocclusion, &reverbocclusion));
    FMODExt_push_float(L, directocclusion);
    FMODExt_push_float(L, reverbocclusion);
    return 2;
}
#endif


/* FMOD_Channel_Set3DSpread(input FMOD_CHANNEL* channel, input float angle, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DSpread
#define FMODExt_func_FMOD_Channel_Set3DSpread _FMODExt_func_FMOD_Channel_Set3DSpread
static int _FMODExt_func_FMOD_Channel_Set3DSpread(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float angle = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_Channel_Set3DSpread, FMOD_RESULT, FMOD_CHANNEL*, float);
    errCheck(FMOD_Channel_Set3DSpread(channel, angle));
    return 0;
}
#endif


/* FMOD_Channel_Get3DSpread(input FMOD_CHANNEL* channel, output float* angle, ) */
#ifndef FMODExt_func_FMOD_Channel_Get3DSpread
#define FMODExt_func_FMOD_Channel_Get3DSpread _FMODExt_func_FMOD_Channel_Get3DSpread
static int _FMODExt_func_FMOD_Channel_Get3DSpread(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float angle;
    ensure(LL, FMOD_Channel_Get3DSpread, FMOD_RESULT, FMOD_CHANNEL*, float*);
    errCheck(FMOD_Channel_Get3DSpread(channel, &angle));
    FMODExt_push_float(L, angle);
    return 1;
}
#endif


/* FMOD_Channel_Set3DLevel(input FMOD_CHANNEL* channel, input float level, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DLevel
#define FMODExt_func_FMOD_Channel_Set3DLevel _FMODExt_func_FMOD_Channel_Set3DLevel
static int _FMODExt_func_FMOD_Channel_Set3DLevel(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float level = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_Channel_Set3DLevel, FMOD_RESULT, FMOD_CHANNEL*, float);
    errCheck(FMOD_Channel_Set3DLevel(channel, level));
    return 0;
}
#endif


/* FMOD_Channel_Get3DLevel(input FMOD_CHANNEL* channel, output float* level, ) */
#ifndef FMODExt_func_FMOD_Channel_Get3DLevel
#define FMODExt_func_FMOD_Channel_Get3DLevel _FMODExt_func_FMOD_Channel_Get3DLevel
static int _FMODExt_func_FMOD_Channel_Get3DLevel(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float level;
    ensure(LL, FMOD_Channel_Get3DLevel, FMOD_RESULT, FMOD_CHANNEL*, float*);
    errCheck(FMOD_Channel_Get3DLevel(channel, &level));
    FMODExt_push_float(L, level);
    return 1;
}
#endif


/* FMOD_Channel_Set3DDopplerLevel(input FMOD_CHANNEL* channel, input float level, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DDopplerLevel
#define FMODExt_func_FMOD_Channel_Set3DDopplerLevel _FMODExt_func_FMOD_Channel_Set3DDopplerLevel
static int _FMODExt_func_FMOD_Channel_Set3DDopplerLevel(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float level = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_Channel_Set3DDopplerLevel, FMOD_RESULT, FMOD_CHANNEL*, float);
    errCheck(FMOD_Channel_Set3DDopplerLevel(channel, level));
    return 0;
}
#endif


/* FMOD_Channel_Get3DDopplerLevel(input FMOD_CHANNEL* channel, output float* level, ) */
#ifndef FMODExt_func_FMOD_Channel_Get3DDopplerLevel
#define FMODExt_func_FMOD_Channel_Get3DDopplerLevel _FMODExt_func_FMOD_Channel_Get3DDopplerLevel
static int _FMODExt_func_FMOD_Channel_Get3DDopplerLevel(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float level;
    ensure(LL, FMOD_Channel_Get3DDopplerLevel, FMOD_RESULT, FMOD_CHANNEL*, float*);
    errCheck(FMOD_Channel_Get3DDopplerLevel(channel, &level));
    FMODExt_push_float(L, level);
    return 1;
}
#endif


/* FMOD_Channel_Set3DDistanceFilter(input FMOD_CHANNEL* channel, input FMOD_BOOL custom, input float customLevel, input float centerFreq, ) */
#ifndef FMODExt_func_FMOD_Channel_Set3DDistanceFilter
#define FMODExt_func_FMOD_Channel_Set3DDistanceFilter _FMODExt_func_FMOD_Channel_Set3DDistanceFilter
static int _FMODExt_func_FMOD_Channel_Set3DDistanceFilter(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL custom = FMODExt_check_FMOD_BOOL(L, 2);
    float customLevel = FMODExt_check_float(L, 3);
    float centerFreq = FMODExt_check_float(L, 4);
    ensure(LL, FMOD_Channel_Set3DDistanceFilter, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL, float, float);
    errCheck(FMOD_Channel_Set3DDistanceFilter(channel, custom, customLevel, centerFreq));
    return 0;
}
#endif


/* FMOD_Channel_Get3DDistanceFilter(input FMOD_CHANNEL* channel, output FMOD_BOOL* custom, output float* customLevel, output float* centerFreq, ) */
#ifndef FMODExt_func_FMOD_Channel_Get3DDistanceFilter
#define FMODExt_func_FMOD_Channel_Get3DDistanceFilter _FMODExt_func_FMOD_Channel_Get3DDistanceFilter
static int _FMODExt_func_FMOD_Channel_Get3DDistanceFilter(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL custom;
    float customLevel;
    float centerFreq;
    ensure(LL, FMOD_Channel_Get3DDistanceFilter, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL*, float*, float*);
    errCheck(FMOD_Channel_Get3DDistanceFilter(channel, &custom, &customLevel, &centerFreq));
    FMODExt_push_FMOD_BOOL(L, custom);
    FMODExt_push_float(L, customLevel);
    FMODExt_push_float(L, centerFreq);
    return 3;
}
#endif


/* FMOD_Channel_SetUserData(input FMOD_CHANNEL* channel, unknown void* userdata, ) */


/* FMOD_Channel_GetUserData(input FMOD_CHANNEL* channel, unknown void** userdata, ) */


/* FMOD_Channel_SetFrequency(input FMOD_CHANNEL* channel, input float frequency, ) */
#ifndef FMODExt_func_FMOD_Channel_SetFrequency
#define FMODExt_func_FMOD_Channel_SetFrequency _FMODExt_func_FMOD_Channel_SetFrequency
static int _FMODExt_func_FMOD_Channel_SetFrequency(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float frequency = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_Channel_SetFrequency, FMOD_RESULT, FMOD_CHANNEL*, float);
    errCheck(FMOD_Channel_SetFrequency(channel, frequency));
    return 0;
}
#endif


/* FMOD_Channel_GetFrequency(input FMOD_CHANNEL* channel, output float* frequency, ) */
#ifndef FMODExt_func_FMOD_Channel_GetFrequency
#define FMODExt_func_FMOD_Channel_GetFrequency _FMODExt_func_FMOD_Channel_GetFrequency
static int _FMODExt_func_FMOD_Channel_GetFrequency(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    float frequency;
    ensure(LL, FMOD_Channel_GetFrequency, FMOD_RESULT, FMOD_CHANNEL*, float*);
    errCheck(FMOD_Channel_GetFrequency(channel, &frequency));
    FMODExt_push_float(L, frequency);
    return 1;
}
#endif


/* FMOD_Channel_SetPriority(input FMOD_CHANNEL* channel, input int priority, ) */
#ifndef FMODExt_func_FMOD_Channel_SetPriority
#define FMODExt_func_FMOD_Channel_SetPriority _FMODExt_func_FMOD_Channel_SetPriority
static int _FMODExt_func_FMOD_Channel_SetPriority(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int priority = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_Channel_SetPriority, FMOD_RESULT, FMOD_CHANNEL*, int);
    errCheck(FMOD_Channel_SetPriority(channel, priority));
    return 0;
}
#endif


/* FMOD_Channel_GetPriority(input FMOD_CHANNEL* channel, output int* priority, ) */
#ifndef FMODExt_func_FMOD_Channel_GetPriority
#define FMODExt_func_FMOD_Channel_GetPriority _FMODExt_func_FMOD_Channel_GetPriority
static int _FMODExt_func_FMOD_Channel_GetPriority(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int priority;
    ensure(LL, FMOD_Channel_GetPriority, FMOD_RESULT, FMOD_CHANNEL*, int*);
    errCheck(FMOD_Channel_GetPriority(channel, &priority));
    FMODExt_push_int(L, priority);
    return 1;
}
#endif


/* FMOD_Channel_SetPosition(input FMOD_CHANNEL* channel, input unsigned int position, input FMOD_TIMEUNIT postype, ) */
#ifndef FMODExt_func_FMOD_Channel_SetPosition
#define FMODExt_func_FMOD_Channel_SetPosition _FMODExt_func_FMOD_Channel_SetPosition
static int _FMODExt_func_FMOD_Channel_SetPosition(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned int position = FMODExt_check_unsigned_int(L, 2);
    FMOD_TIMEUNIT postype = FMODExt_check_unsigned_int(L, 3);
    ensure(LL, FMOD_Channel_SetPosition, FMOD_RESULT, FMOD_CHANNEL*, unsigned int, FMOD_TIMEUNIT);
    errCheck(FMOD_Channel_SetPosition(channel, position, postype));
    return 0;
}
#endif


/* FMOD_Channel_GetPosition(input FMOD_CHANNEL* channel, output unsigned int* position, input FMOD_TIMEUNIT postype, ) */
#ifndef FMODExt_func_FMOD_Channel_GetPosition
#define FMODExt_func_FMOD_Channel_GetPosition _FMODExt_func_FMOD_Channel_GetPosition
static int _FMODExt_func_FMOD_Channel_GetPosition(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned int position;
    FMOD_TIMEUNIT postype = FMODExt_check_unsigned_int(L, 2);
    ensure(LL, FMOD_Channel_GetPosition, FMOD_RESULT, FMOD_CHANNEL*, unsigned int*, FMOD_TIMEUNIT);
    errCheck(FMOD_Channel_GetPosition(channel, &position, postype));
    FMODExt_push_unsigned_int(L, position);
    return 1;
}
#endif


/* FMOD_Channel_SetChannelGroup(input FMOD_CHANNEL* channel, input FMOD_CHANNELGROUP* channelgroup, ) */
#ifndef FMODExt_func_FMOD_Channel_SetChannelGroup
#define FMODExt_func_FMOD_Channel_SetChannelGroup _FMODExt_func_FMOD_Channel_SetChannelGroup
static int _FMODExt_func_FMOD_Channel_SetChannelGroup(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 2);
    ensure(LL, FMOD_Channel_SetChannelGroup, FMOD_RESULT, FMOD_CHANNEL*, FMOD_CHANNELGROUP*);
    errCheck(FMOD_Channel_SetChannelGroup(channel, channelgroup));
    return 0;
}
#endif


/* FMOD_Channel_GetChannelGroup(input FMOD_CHANNEL* channel, output FMOD_CHANNELGROUP** channelgroup, ) */
#ifndef FMODExt_func_FMOD_Channel_GetChannelGroup
#define FMODExt_func_FMOD_Channel_GetChannelGroup _FMODExt_func_FMOD_Channel_GetChannelGroup
static int _FMODExt_func_FMOD_Channel_GetChannelGroup(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_CHANNELGROUP* channelgroup;
    ensure(LL, FMOD_Channel_GetChannelGroup, FMOD_RESULT, FMOD_CHANNEL*, FMOD_CHANNELGROUP**);
    errCheck(FMOD_Channel_GetChannelGroup(channel, &channelgroup));
    FMODExt_push_ptr_FMOD_CHANNELGROUP(L, channelgroup);
    return 1;
}
#endif


/* FMOD_Channel_SetLoopCount(input FMOD_CHANNEL* channel, input int loopcount, ) */
#ifndef FMODExt_func_FMOD_Channel_SetLoopCount
#define FMODExt_func_FMOD_Channel_SetLoopCount _FMODExt_func_FMOD_Channel_SetLoopCount
static int _FMODExt_func_FMOD_Channel_SetLoopCount(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int loopcount = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_Channel_SetLoopCount, FMOD_RESULT, FMOD_CHANNEL*, int);
    errCheck(FMOD_Channel_SetLoopCount(channel, loopcount));
    return 0;
}
#endif


/* FMOD_Channel_GetLoopCount(input FMOD_CHANNEL* channel, output int* loopcount, ) */
#ifndef FMODExt_func_FMOD_Channel_GetLoopCount
#define FMODExt_func_FMOD_Channel_GetLoopCount _FMODExt_func_FMOD_Channel_GetLoopCount
static int _FMODExt_func_FMOD_Channel_GetLoopCount(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int loopcount;
    ensure(LL, FMOD_Channel_GetLoopCount, FMOD_RESULT, FMOD_CHANNEL*, int*);
    errCheck(FMOD_Channel_GetLoopCount(channel, &loopcount));
    FMODExt_push_int(L, loopcount);
    return 1;
}
#endif


/* FMOD_Channel_SetLoopPoints(input FMOD_CHANNEL* channel, input unsigned int loopstart, input FMOD_TIMEUNIT loopstarttype, input unsigned int loopend, input FMOD_TIMEUNIT loopendtype, ) */
#ifndef FMODExt_func_FMOD_Channel_SetLoopPoints
#define FMODExt_func_FMOD_Channel_SetLoopPoints _FMODExt_func_FMOD_Channel_SetLoopPoints
static int _FMODExt_func_FMOD_Channel_SetLoopPoints(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned int loopstart = FMODExt_check_unsigned_int(L, 2);
    FMOD_TIMEUNIT loopstarttype = FMODExt_check_unsigned_int(L, 3);
    unsigned int loopend = FMODExt_check_unsigned_int(L, 4);
    FMOD_TIMEUNIT loopendtype = FMODExt_check_unsigned_int(L, 5);
    ensure(LL, FMOD_Channel_SetLoopPoints, FMOD_RESULT, FMOD_CHANNEL*, unsigned int, FMOD_TIMEUNIT, unsigned int, FMOD_TIMEUNIT);
    errCheck(FMOD_Channel_SetLoopPoints(channel, loopstart, loopstarttype, loopend, loopendtype));
    return 0;
}
#endif


/* FMOD_Channel_GetLoopPoints(input FMOD_CHANNEL* channel, output unsigned int* loopstart, input FMOD_TIMEUNIT loopstarttype, output unsigned int* loopend, input FMOD_TIMEUNIT loopendtype, ) */
#ifndef FMODExt_func_FMOD_Channel_GetLoopPoints
#define FMODExt_func_FMOD_Channel_GetLoopPoints _FMODExt_func_FMOD_Channel_GetLoopPoints
static int _FMODExt_func_FMOD_Channel_GetLoopPoints(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    unsigned int loopstart;
    FMOD_TIMEUNIT loopstarttype = FMODExt_check_unsigned_int(L, 2);
    unsigned int loopend;
    FMOD_TIMEUNIT loopendtype = FMODExt_check_unsigned_int(L, 3);
    ensure(LL, FMOD_Channel_GetLoopPoints, FMOD_RESULT, FMOD_CHANNEL*, unsigned int*, FMOD_TIMEUNIT, unsigned int*, FMOD_TIMEUNIT);
    errCheck(FMOD_Channel_GetLoopPoints(channel, &loopstart, loopstarttype, &loopend, loopendtype));
    FMODExt_push_unsigned_int(L, loopstart);
    FMODExt_push_unsigned_int(L, loopend);
    return 2;
}
#endif


/* FMOD_Channel_IsVirtual(input FMOD_CHANNEL* channel, output FMOD_BOOL* isvirtual, ) */
#ifndef FMODExt_func_FMOD_Channel_IsVirtual
#define FMODExt_func_FMOD_Channel_IsVirtual _FMODExt_func_FMOD_Channel_IsVirtual
static int _FMODExt_func_FMOD_Channel_IsVirtual(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_BOOL isvirtual;
    ensure(LL, FMOD_Channel_IsVirtual, FMOD_RESULT, FMOD_CHANNEL*, FMOD_BOOL*);
    errCheck(FMOD_Channel_IsVirtual(channel, &isvirtual));
    FMODExt_push_FMOD_BOOL(L, isvirtual);
    return 1;
}
#endif


/* FMOD_Channel_GetCurrentSound(input FMOD_CHANNEL* channel, output FMOD_SOUND** sound, ) */
#ifndef FMODExt_func_FMOD_Channel_GetCurrentSound
#define FMODExt_func_FMOD_Channel_GetCurrentSound _FMODExt_func_FMOD_Channel_GetCurrentSound
static int _FMODExt_func_FMOD_Channel_GetCurrentSound(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    FMOD_SOUND* sound;
    ensure(LL, FMOD_Channel_GetCurrentSound, FMOD_RESULT, FMOD_CHANNEL*, FMOD_SOUND**);
    errCheck(FMOD_Channel_GetCurrentSound(channel, &sound));
    FMODExt_push_ptr_FMOD_SOUND(L, sound);
    return 1;
}
#endif


/* FMOD_Channel_GetIndex(input FMOD_CHANNEL* channel, output int* index, ) */
#ifndef FMODExt_func_FMOD_Channel_GetIndex
#define FMODExt_func_FMOD_Channel_GetIndex _FMODExt_func_FMOD_Channel_GetIndex
static int _FMODExt_func_FMOD_Channel_GetIndex(lua_State *L) {
    FMOD_CHANNEL* channel = FMODExt_check_ptr_FMOD_CHANNEL(L, 1);
    int index;
    ensure(LL, FMOD_Channel_GetIndex, FMOD_RESULT, FMOD_CHANNEL*, int*);
    errCheck(FMOD_Channel_GetIndex(channel, &index));
    FMODExt_push_int(L, index);
    return 1;
}
#endif


/* FMOD_ChannelGroup_GetSystemObject(input FMOD_CHANNELGROUP* channelgroup, output FMOD_SYSTEM** system, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetSystemObject
#define FMODExt_func_FMOD_ChannelGroup_GetSystemObject _FMODExt_func_FMOD_ChannelGroup_GetSystemObject
static int _FMODExt_func_FMOD_ChannelGroup_GetSystemObject(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_SYSTEM* system;
    ensure(LL, FMOD_ChannelGroup_GetSystemObject, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_SYSTEM**);
    errCheck(FMOD_ChannelGroup_GetSystemObject(channelgroup, &system));
    FMODExt_push_ptr_FMOD_SYSTEM(L, system);
    return 1;
}
#endif


/* FMOD_ChannelGroup_Stop(input FMOD_CHANNELGROUP* channelgroup, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Stop
#define FMODExt_func_FMOD_ChannelGroup_Stop _FMODExt_func_FMOD_ChannelGroup_Stop
static int _FMODExt_func_FMOD_ChannelGroup_Stop(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    ensure(LL, FMOD_ChannelGroup_Stop, FMOD_RESULT, FMOD_CHANNELGROUP*);
    errCheck(FMOD_ChannelGroup_Stop(channelgroup));
    return 0;
}
#endif


/* FMOD_ChannelGroup_SetPaused(input FMOD_CHANNELGROUP* channelgroup, input FMOD_BOOL paused, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetPaused
#define FMODExt_func_FMOD_ChannelGroup_SetPaused _FMODExt_func_FMOD_ChannelGroup_SetPaused
static int _FMODExt_func_FMOD_ChannelGroup_SetPaused(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_BOOL paused = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_ChannelGroup_SetPaused, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_BOOL);
    errCheck(FMOD_ChannelGroup_SetPaused(channelgroup, paused));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetPaused(input FMOD_CHANNELGROUP* channelgroup, output FMOD_BOOL* paused, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetPaused
#define FMODExt_func_FMOD_ChannelGroup_GetPaused _FMODExt_func_FMOD_ChannelGroup_GetPaused
static int _FMODExt_func_FMOD_ChannelGroup_GetPaused(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_BOOL paused;
    ensure(LL, FMOD_ChannelGroup_GetPaused, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_BOOL*);
    errCheck(FMOD_ChannelGroup_GetPaused(channelgroup, &paused));
    FMODExt_push_FMOD_BOOL(L, paused);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetVolume(input FMOD_CHANNELGROUP* channelgroup, input float volume, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetVolume
#define FMODExt_func_FMOD_ChannelGroup_SetVolume _FMODExt_func_FMOD_ChannelGroup_SetVolume
static int _FMODExt_func_FMOD_ChannelGroup_SetVolume(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float volume = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_ChannelGroup_SetVolume, FMOD_RESULT, FMOD_CHANNELGROUP*, float);
    errCheck(FMOD_ChannelGroup_SetVolume(channelgroup, volume));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetVolume(input FMOD_CHANNELGROUP* channelgroup, output float* volume, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetVolume
#define FMODExt_func_FMOD_ChannelGroup_GetVolume _FMODExt_func_FMOD_ChannelGroup_GetVolume
static int _FMODExt_func_FMOD_ChannelGroup_GetVolume(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float volume;
    ensure(LL, FMOD_ChannelGroup_GetVolume, FMOD_RESULT, FMOD_CHANNELGROUP*, float*);
    errCheck(FMOD_ChannelGroup_GetVolume(channelgroup, &volume));
    FMODExt_push_float(L, volume);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetVolumeRamp(input FMOD_CHANNELGROUP* channelgroup, input FMOD_BOOL ramp, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetVolumeRamp
#define FMODExt_func_FMOD_ChannelGroup_SetVolumeRamp _FMODExt_func_FMOD_ChannelGroup_SetVolumeRamp
static int _FMODExt_func_FMOD_ChannelGroup_SetVolumeRamp(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_BOOL ramp = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_ChannelGroup_SetVolumeRamp, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_BOOL);
    errCheck(FMOD_ChannelGroup_SetVolumeRamp(channelgroup, ramp));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetVolumeRamp(input FMOD_CHANNELGROUP* channelgroup, output FMOD_BOOL* ramp, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetVolumeRamp
#define FMODExt_func_FMOD_ChannelGroup_GetVolumeRamp _FMODExt_func_FMOD_ChannelGroup_GetVolumeRamp
static int _FMODExt_func_FMOD_ChannelGroup_GetVolumeRamp(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_BOOL ramp;
    ensure(LL, FMOD_ChannelGroup_GetVolumeRamp, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_BOOL*);
    errCheck(FMOD_ChannelGroup_GetVolumeRamp(channelgroup, &ramp));
    FMODExt_push_FMOD_BOOL(L, ramp);
    return 1;
}
#endif


/* FMOD_ChannelGroup_GetAudibility(input FMOD_CHANNELGROUP* channelgroup, output float* audibility, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetAudibility
#define FMODExt_func_FMOD_ChannelGroup_GetAudibility _FMODExt_func_FMOD_ChannelGroup_GetAudibility
static int _FMODExt_func_FMOD_ChannelGroup_GetAudibility(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float audibility;
    ensure(LL, FMOD_ChannelGroup_GetAudibility, FMOD_RESULT, FMOD_CHANNELGROUP*, float*);
    errCheck(FMOD_ChannelGroup_GetAudibility(channelgroup, &audibility));
    FMODExt_push_float(L, audibility);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetPitch(input FMOD_CHANNELGROUP* channelgroup, input float pitch, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetPitch
#define FMODExt_func_FMOD_ChannelGroup_SetPitch _FMODExt_func_FMOD_ChannelGroup_SetPitch
static int _FMODExt_func_FMOD_ChannelGroup_SetPitch(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float pitch = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_ChannelGroup_SetPitch, FMOD_RESULT, FMOD_CHANNELGROUP*, float);
    errCheck(FMOD_ChannelGroup_SetPitch(channelgroup, pitch));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetPitch(input FMOD_CHANNELGROUP* channelgroup, output float* pitch, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetPitch
#define FMODExt_func_FMOD_ChannelGroup_GetPitch _FMODExt_func_FMOD_ChannelGroup_GetPitch
static int _FMODExt_func_FMOD_ChannelGroup_GetPitch(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float pitch;
    ensure(LL, FMOD_ChannelGroup_GetPitch, FMOD_RESULT, FMOD_CHANNELGROUP*, float*);
    errCheck(FMOD_ChannelGroup_GetPitch(channelgroup, &pitch));
    FMODExt_push_float(L, pitch);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetMute(input FMOD_CHANNELGROUP* channelgroup, input FMOD_BOOL mute, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetMute
#define FMODExt_func_FMOD_ChannelGroup_SetMute _FMODExt_func_FMOD_ChannelGroup_SetMute
static int _FMODExt_func_FMOD_ChannelGroup_SetMute(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_BOOL mute = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_ChannelGroup_SetMute, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_BOOL);
    errCheck(FMOD_ChannelGroup_SetMute(channelgroup, mute));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetMute(input FMOD_CHANNELGROUP* channelgroup, output FMOD_BOOL* mute, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetMute
#define FMODExt_func_FMOD_ChannelGroup_GetMute _FMODExt_func_FMOD_ChannelGroup_GetMute
static int _FMODExt_func_FMOD_ChannelGroup_GetMute(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_BOOL mute;
    ensure(LL, FMOD_ChannelGroup_GetMute, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_BOOL*);
    errCheck(FMOD_ChannelGroup_GetMute(channelgroup, &mute));
    FMODExt_push_FMOD_BOOL(L, mute);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetReverbProperties(input FMOD_CHANNELGROUP* channelgroup, input int instance, input float wet, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetReverbProperties
#define FMODExt_func_FMOD_ChannelGroup_SetReverbProperties _FMODExt_func_FMOD_ChannelGroup_SetReverbProperties
static int _FMODExt_func_FMOD_ChannelGroup_SetReverbProperties(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    int instance = FMODExt_check_int(L, 2);
    float wet = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_ChannelGroup_SetReverbProperties, FMOD_RESULT, FMOD_CHANNELGROUP*, int, float);
    errCheck(FMOD_ChannelGroup_SetReverbProperties(channelgroup, instance, wet));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetReverbProperties(input FMOD_CHANNELGROUP* channelgroup, input int instance, output float* wet, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetReverbProperties
#define FMODExt_func_FMOD_ChannelGroup_GetReverbProperties _FMODExt_func_FMOD_ChannelGroup_GetReverbProperties
static int _FMODExt_func_FMOD_ChannelGroup_GetReverbProperties(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    int instance = FMODExt_check_int(L, 2);
    float wet;
    ensure(LL, FMOD_ChannelGroup_GetReverbProperties, FMOD_RESULT, FMOD_CHANNELGROUP*, int, float*);
    errCheck(FMOD_ChannelGroup_GetReverbProperties(channelgroup, instance, &wet));
    FMODExt_push_float(L, wet);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetLowPassGain(input FMOD_CHANNELGROUP* channelgroup, input float gain, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetLowPassGain
#define FMODExt_func_FMOD_ChannelGroup_SetLowPassGain _FMODExt_func_FMOD_ChannelGroup_SetLowPassGain
static int _FMODExt_func_FMOD_ChannelGroup_SetLowPassGain(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float gain = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_ChannelGroup_SetLowPassGain, FMOD_RESULT, FMOD_CHANNELGROUP*, float);
    errCheck(FMOD_ChannelGroup_SetLowPassGain(channelgroup, gain));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetLowPassGain(input FMOD_CHANNELGROUP* channelgroup, output float* gain, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetLowPassGain
#define FMODExt_func_FMOD_ChannelGroup_GetLowPassGain _FMODExt_func_FMOD_ChannelGroup_GetLowPassGain
static int _FMODExt_func_FMOD_ChannelGroup_GetLowPassGain(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float gain;
    ensure(LL, FMOD_ChannelGroup_GetLowPassGain, FMOD_RESULT, FMOD_CHANNELGROUP*, float*);
    errCheck(FMOD_ChannelGroup_GetLowPassGain(channelgroup, &gain));
    FMODExt_push_float(L, gain);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetMode(input FMOD_CHANNELGROUP* channelgroup, input FMOD_MODE mode, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetMode
#define FMODExt_func_FMOD_ChannelGroup_SetMode _FMODExt_func_FMOD_ChannelGroup_SetMode
static int _FMODExt_func_FMOD_ChannelGroup_SetMode(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_MODE mode = FMODExt_check_unsigned_int(L, 2);
    ensure(LL, FMOD_ChannelGroup_SetMode, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_MODE);
    errCheck(FMOD_ChannelGroup_SetMode(channelgroup, mode));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetMode(input FMOD_CHANNELGROUP* channelgroup, output FMOD_MODE* mode, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetMode
#define FMODExt_func_FMOD_ChannelGroup_GetMode _FMODExt_func_FMOD_ChannelGroup_GetMode
static int _FMODExt_func_FMOD_ChannelGroup_GetMode(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_MODE mode;
    ensure(LL, FMOD_ChannelGroup_GetMode, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_MODE*);
    errCheck(FMOD_ChannelGroup_GetMode(channelgroup, &mode));
    FMODExt_push_unsigned_int(L, mode);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetCallback(input FMOD_CHANNELGROUP* channelgroup, unknown FMOD_CHANNELCONTROL_CALLBACK callback, ) */


/* FMOD_ChannelGroup_IsPlaying(input FMOD_CHANNELGROUP* channelgroup, output FMOD_BOOL* isplaying, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_IsPlaying
#define FMODExt_func_FMOD_ChannelGroup_IsPlaying _FMODExt_func_FMOD_ChannelGroup_IsPlaying
static int _FMODExt_func_FMOD_ChannelGroup_IsPlaying(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_BOOL isplaying;
    ensure(LL, FMOD_ChannelGroup_IsPlaying, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_BOOL*);
    errCheck(FMOD_ChannelGroup_IsPlaying(channelgroup, &isplaying));
    FMODExt_push_FMOD_BOOL(L, isplaying);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetPan(input FMOD_CHANNELGROUP* channelgroup, input float pan, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetPan
#define FMODExt_func_FMOD_ChannelGroup_SetPan _FMODExt_func_FMOD_ChannelGroup_SetPan
static int _FMODExt_func_FMOD_ChannelGroup_SetPan(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float pan = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_ChannelGroup_SetPan, FMOD_RESULT, FMOD_CHANNELGROUP*, float);
    errCheck(FMOD_ChannelGroup_SetPan(channelgroup, pan));
    return 0;
}
#endif


/* FMOD_ChannelGroup_SetMixLevelsOutput(input FMOD_CHANNELGROUP* channelgroup, input float frontleft, input float frontright, input float center, input float lfe, input float surroundleft, input float surroundright, input float backleft, input float backright, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetMixLevelsOutput
#define FMODExt_func_FMOD_ChannelGroup_SetMixLevelsOutput _FMODExt_func_FMOD_ChannelGroup_SetMixLevelsOutput
static int _FMODExt_func_FMOD_ChannelGroup_SetMixLevelsOutput(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float frontleft = FMODExt_check_float(L, 2);
    float frontright = FMODExt_check_float(L, 3);
    float center = FMODExt_check_float(L, 4);
    float lfe = FMODExt_check_float(L, 5);
    float surroundleft = FMODExt_check_float(L, 6);
    float surroundright = FMODExt_check_float(L, 7);
    float backleft = FMODExt_check_float(L, 8);
    float backright = FMODExt_check_float(L, 9);
    ensure(LL, FMOD_ChannelGroup_SetMixLevelsOutput, FMOD_RESULT, FMOD_CHANNELGROUP*, float, float, float, float, float, float, float, float);
    errCheck(FMOD_ChannelGroup_SetMixLevelsOutput(channelgroup, frontleft, frontright, center, lfe, surroundleft, surroundright, backleft, backright));
    return 0;
}
#endif


/* FMOD_ChannelGroup_SetMixLevelsInput(input FMOD_CHANNELGROUP* channelgroup, output float* levels, input int numlevels, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetMixLevelsInput
#define FMODExt_func_FMOD_ChannelGroup_SetMixLevelsInput _FMODExt_func_FMOD_ChannelGroup_SetMixLevelsInput
static int _FMODExt_func_FMOD_ChannelGroup_SetMixLevelsInput(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float levels;
    int numlevels = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_ChannelGroup_SetMixLevelsInput, FMOD_RESULT, FMOD_CHANNELGROUP*, float*, int);
    errCheck(FMOD_ChannelGroup_SetMixLevelsInput(channelgroup, &levels, numlevels));
    FMODExt_push_float(L, levels);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetMixMatrix(input FMOD_CHANNELGROUP* channelgroup, output float* matrix, input int outchannels, input int inchannels, input int inchannel_hop, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetMixMatrix
#define FMODExt_func_FMOD_ChannelGroup_SetMixMatrix _FMODExt_func_FMOD_ChannelGroup_SetMixMatrix
static int _FMODExt_func_FMOD_ChannelGroup_SetMixMatrix(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float matrix;
    int outchannels = FMODExt_check_int(L, 2);
    int inchannels = FMODExt_check_int(L, 3);
    int inchannel_hop = FMODExt_check_int(L, 4);
    ensure(LL, FMOD_ChannelGroup_SetMixMatrix, FMOD_RESULT, FMOD_CHANNELGROUP*, float*, int, int, int);
    errCheck(FMOD_ChannelGroup_SetMixMatrix(channelgroup, &matrix, outchannels, inchannels, inchannel_hop));
    FMODExt_push_float(L, matrix);
    return 1;
}
#endif


/* FMOD_ChannelGroup_GetMixMatrix(input FMOD_CHANNELGROUP* channelgroup, output float* matrix, output int* outchannels, output int* inchannels, input int inchannel_hop, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetMixMatrix
#define FMODExt_func_FMOD_ChannelGroup_GetMixMatrix _FMODExt_func_FMOD_ChannelGroup_GetMixMatrix
static int _FMODExt_func_FMOD_ChannelGroup_GetMixMatrix(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float matrix;
    int outchannels;
    int inchannels;
    int inchannel_hop = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_ChannelGroup_GetMixMatrix, FMOD_RESULT, FMOD_CHANNELGROUP*, float*, int*, int*, int);
    errCheck(FMOD_ChannelGroup_GetMixMatrix(channelgroup, &matrix, &outchannels, &inchannels, inchannel_hop));
    FMODExt_push_float(L, matrix);
    FMODExt_push_int(L, outchannels);
    FMODExt_push_int(L, inchannels);
    return 3;
}
#endif


/* FMOD_ChannelGroup_GetDSPClock(input FMOD_CHANNELGROUP* channelgroup, output unsigned long long* dspclock, output unsigned long long* parentclock, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetDSPClock
#define FMODExt_func_FMOD_ChannelGroup_GetDSPClock _FMODExt_func_FMOD_ChannelGroup_GetDSPClock
static int _FMODExt_func_FMOD_ChannelGroup_GetDSPClock(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    unsigned long long dspclock;
    unsigned long long parentclock;
    ensure(LL, FMOD_ChannelGroup_GetDSPClock, FMOD_RESULT, FMOD_CHANNELGROUP*, unsigned long long*, unsigned long long*);
    errCheck(FMOD_ChannelGroup_GetDSPClock(channelgroup, &dspclock, &parentclock));
    FMODExt_push_unsigned_long_long(L, dspclock);
    FMODExt_push_unsigned_long_long(L, parentclock);
    return 2;
}
#endif


/* FMOD_ChannelGroup_SetDelay(input FMOD_CHANNELGROUP* channelgroup, input unsigned long long dspclock_start, input unsigned long long dspclock_end, input FMOD_BOOL stopchannels, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetDelay
#define FMODExt_func_FMOD_ChannelGroup_SetDelay _FMODExt_func_FMOD_ChannelGroup_SetDelay
static int _FMODExt_func_FMOD_ChannelGroup_SetDelay(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    unsigned long long dspclock_start = FMODExt_check_unsigned_long_long(L, 2);
    unsigned long long dspclock_end = FMODExt_check_unsigned_long_long(L, 3);
    FMOD_BOOL stopchannels = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(LL, FMOD_ChannelGroup_SetDelay, FMOD_RESULT, FMOD_CHANNELGROUP*, unsigned long long, unsigned long long, FMOD_BOOL);
    errCheck(FMOD_ChannelGroup_SetDelay(channelgroup, dspclock_start, dspclock_end, stopchannels));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetDelay(input FMOD_CHANNELGROUP* channelgroup, output unsigned long long* dspclock_start, output unsigned long long* dspclock_end, output FMOD_BOOL* stopchannels, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetDelay
#define FMODExt_func_FMOD_ChannelGroup_GetDelay _FMODExt_func_FMOD_ChannelGroup_GetDelay
static int _FMODExt_func_FMOD_ChannelGroup_GetDelay(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    unsigned long long dspclock_start;
    unsigned long long dspclock_end;
    FMOD_BOOL stopchannels;
    ensure(LL, FMOD_ChannelGroup_GetDelay, FMOD_RESULT, FMOD_CHANNELGROUP*, unsigned long long*, unsigned long long*, FMOD_BOOL*);
    errCheck(FMOD_ChannelGroup_GetDelay(channelgroup, &dspclock_start, &dspclock_end, &stopchannels));
    FMODExt_push_unsigned_long_long(L, dspclock_start);
    FMODExt_push_unsigned_long_long(L, dspclock_end);
    FMODExt_push_FMOD_BOOL(L, stopchannels);
    return 3;
}
#endif


/* FMOD_ChannelGroup_AddFadePoint(input FMOD_CHANNELGROUP* channelgroup, input unsigned long long dspclock, input float volume, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_AddFadePoint
#define FMODExt_func_FMOD_ChannelGroup_AddFadePoint _FMODExt_func_FMOD_ChannelGroup_AddFadePoint
static int _FMODExt_func_FMOD_ChannelGroup_AddFadePoint(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    unsigned long long dspclock = FMODExt_check_unsigned_long_long(L, 2);
    float volume = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_ChannelGroup_AddFadePoint, FMOD_RESULT, FMOD_CHANNELGROUP*, unsigned long long, float);
    errCheck(FMOD_ChannelGroup_AddFadePoint(channelgroup, dspclock, volume));
    return 0;
}
#endif


/* FMOD_ChannelGroup_SetFadePointRamp(input FMOD_CHANNELGROUP* channelgroup, input unsigned long long dspclock, input float volume, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetFadePointRamp
#define FMODExt_func_FMOD_ChannelGroup_SetFadePointRamp _FMODExt_func_FMOD_ChannelGroup_SetFadePointRamp
static int _FMODExt_func_FMOD_ChannelGroup_SetFadePointRamp(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    unsigned long long dspclock = FMODExt_check_unsigned_long_long(L, 2);
    float volume = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_ChannelGroup_SetFadePointRamp, FMOD_RESULT, FMOD_CHANNELGROUP*, unsigned long long, float);
    errCheck(FMOD_ChannelGroup_SetFadePointRamp(channelgroup, dspclock, volume));
    return 0;
}
#endif


/* FMOD_ChannelGroup_RemoveFadePoints(input FMOD_CHANNELGROUP* channelgroup, input unsigned long long dspclock_start, input unsigned long long dspclock_end, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_RemoveFadePoints
#define FMODExt_func_FMOD_ChannelGroup_RemoveFadePoints _FMODExt_func_FMOD_ChannelGroup_RemoveFadePoints
static int _FMODExt_func_FMOD_ChannelGroup_RemoveFadePoints(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    unsigned long long dspclock_start = FMODExt_check_unsigned_long_long(L, 2);
    unsigned long long dspclock_end = FMODExt_check_unsigned_long_long(L, 3);
    ensure(LL, FMOD_ChannelGroup_RemoveFadePoints, FMOD_RESULT, FMOD_CHANNELGROUP*, unsigned long long, unsigned long long);
    errCheck(FMOD_ChannelGroup_RemoveFadePoints(channelgroup, dspclock_start, dspclock_end));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetFadePoints(input FMOD_CHANNELGROUP* channelgroup, output unsigned int* numpoints, output unsigned long long* point_dspclock, output float* point_volume, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetFadePoints
#define FMODExt_func_FMOD_ChannelGroup_GetFadePoints _FMODExt_func_FMOD_ChannelGroup_GetFadePoints
static int _FMODExt_func_FMOD_ChannelGroup_GetFadePoints(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    unsigned int numpoints;
    unsigned long long point_dspclock;
    float point_volume;
    ensure(LL, FMOD_ChannelGroup_GetFadePoints, FMOD_RESULT, FMOD_CHANNELGROUP*, unsigned int*, unsigned long long*, float*);
    errCheck(FMOD_ChannelGroup_GetFadePoints(channelgroup, &numpoints, &point_dspclock, &point_volume));
    FMODExt_push_unsigned_int(L, numpoints);
    FMODExt_push_unsigned_long_long(L, point_dspclock);
    FMODExt_push_float(L, point_volume);
    return 3;
}
#endif


/* FMOD_ChannelGroup_GetDSP(input FMOD_CHANNELGROUP* channelgroup, input int index, output FMOD_DSP** dsp, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetDSP
#define FMODExt_func_FMOD_ChannelGroup_GetDSP _FMODExt_func_FMOD_ChannelGroup_GetDSP
static int _FMODExt_func_FMOD_ChannelGroup_GetDSP(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_DSP* dsp;
    ensure(LL, FMOD_ChannelGroup_GetDSP, FMOD_RESULT, FMOD_CHANNELGROUP*, int, FMOD_DSP**);
    errCheck(FMOD_ChannelGroup_GetDSP(channelgroup, index, &dsp));
    FMODExt_push_ptr_FMOD_DSP(L, dsp);
    return 1;
}
#endif


/* FMOD_ChannelGroup_AddDSP(input FMOD_CHANNELGROUP* channelgroup, input int index, input FMOD_DSP* dsp, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_AddDSP
#define FMODExt_func_FMOD_ChannelGroup_AddDSP _FMODExt_func_FMOD_ChannelGroup_AddDSP
static int _FMODExt_func_FMOD_ChannelGroup_AddDSP(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 3);
    ensure(LL, FMOD_ChannelGroup_AddDSP, FMOD_RESULT, FMOD_CHANNELGROUP*, int, FMOD_DSP*);
    errCheck(FMOD_ChannelGroup_AddDSP(channelgroup, index, dsp));
    return 0;
}
#endif


/* FMOD_ChannelGroup_RemoveDSP(input FMOD_CHANNELGROUP* channelgroup, input FMOD_DSP* dsp, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_RemoveDSP
#define FMODExt_func_FMOD_ChannelGroup_RemoveDSP _FMODExt_func_FMOD_ChannelGroup_RemoveDSP
static int _FMODExt_func_FMOD_ChannelGroup_RemoveDSP(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 2);
    ensure(LL, FMOD_ChannelGroup_RemoveDSP, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_DSP*);
    errCheck(FMOD_ChannelGroup_RemoveDSP(channelgroup, dsp));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetNumDSPs(input FMOD_CHANNELGROUP* channelgroup, output int* numdsps, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetNumDSPs
#define FMODExt_func_FMOD_ChannelGroup_GetNumDSPs _FMODExt_func_FMOD_ChannelGroup_GetNumDSPs
static int _FMODExt_func_FMOD_ChannelGroup_GetNumDSPs(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    int numdsps;
    ensure(LL, FMOD_ChannelGroup_GetNumDSPs, FMOD_RESULT, FMOD_CHANNELGROUP*, int*);
    errCheck(FMOD_ChannelGroup_GetNumDSPs(channelgroup, &numdsps));
    FMODExt_push_int(L, numdsps);
    return 1;
}
#endif


/* FMOD_ChannelGroup_SetDSPIndex(input FMOD_CHANNELGROUP* channelgroup, input FMOD_DSP* dsp, input int index, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_SetDSPIndex
#define FMODExt_func_FMOD_ChannelGroup_SetDSPIndex _FMODExt_func_FMOD_ChannelGroup_SetDSPIndex
static int _FMODExt_func_FMOD_ChannelGroup_SetDSPIndex(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 2);
    int index = FMODExt_check_int(L, 3);
    ensure(LL, FMOD_ChannelGroup_SetDSPIndex, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_DSP*, int);
    errCheck(FMOD_ChannelGroup_SetDSPIndex(channelgroup, dsp, index));
    return 0;
}
#endif


/* FMOD_ChannelGroup_GetDSPIndex(input FMOD_CHANNELGROUP* channelgroup, input FMOD_DSP* dsp, output int* index, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetDSPIndex
#define FMODExt_func_FMOD_ChannelGroup_GetDSPIndex _FMODExt_func_FMOD_ChannelGroup_GetDSPIndex
static int _FMODExt_func_FMOD_ChannelGroup_GetDSPIndex(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 2);
    int index;
    ensure(LL, FMOD_ChannelGroup_GetDSPIndex, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_DSP*, int*);
    errCheck(FMOD_ChannelGroup_GetDSPIndex(channelgroup, dsp, &index));
    FMODExt_push_int(L, index);
    return 1;
}
#endif


/* FMOD_ChannelGroup_Set3DAttributes(input FMOD_CHANNELGROUP* channelgroup, input_ptr const FMOD_VECTOR* pos, input_ptr const FMOD_VECTOR* vel, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DAttributes
#define FMODExt_func_FMOD_ChannelGroup_Set3DAttributes _FMODExt_func_FMOD_ChannelGroup_Set3DAttributes
static int _FMODExt_func_FMOD_ChannelGroup_Set3DAttributes(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    const FMOD_VECTOR pos = FMODExt_check_FMOD_VECTOR(L, 2);
    const FMOD_VECTOR vel = FMODExt_check_FMOD_VECTOR(L, 3);
    ensure(LL, FMOD_ChannelGroup_Set3DAttributes, FMOD_RESULT, FMOD_CHANNELGROUP*, const FMOD_VECTOR*, const FMOD_VECTOR*);
    errCheck(FMOD_ChannelGroup_Set3DAttributes(channelgroup, &pos, &vel));
    return 0;
}
#endif


/* FMOD_ChannelGroup_Get3DAttributes(input FMOD_CHANNELGROUP* channelgroup, output FMOD_VECTOR* pos, output FMOD_VECTOR* vel, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Get3DAttributes
#define FMODExt_func_FMOD_ChannelGroup_Get3DAttributes _FMODExt_func_FMOD_ChannelGroup_Get3DAttributes
static int _FMODExt_func_FMOD_ChannelGroup_Get3DAttributes(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_VECTOR pos;
    FMOD_VECTOR vel;
    ensure(LL, FMOD_ChannelGroup_Get3DAttributes, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_VECTOR*, FMOD_VECTOR*);
    errCheck(FMOD_ChannelGroup_Get3DAttributes(channelgroup, &pos, &vel));
    FMODExt_push_FMOD_VECTOR(L, pos);
    FMODExt_push_FMOD_VECTOR(L, vel);
    return 2;
}
#endif


/* FMOD_ChannelGroup_Set3DMinMaxDistance(input FMOD_CHANNELGROUP* channelgroup, input float mindistance, input float maxdistance, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DMinMaxDistance
#define FMODExt_func_FMOD_ChannelGroup_Set3DMinMaxDistance _FMODExt_func_FMOD_ChannelGroup_Set3DMinMaxDistance
static int _FMODExt_func_FMOD_ChannelGroup_Set3DMinMaxDistance(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float mindistance = FMODExt_check_float(L, 2);
    float maxdistance = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_ChannelGroup_Set3DMinMaxDistance, FMOD_RESULT, FMOD_CHANNELGROUP*, float, float);
    errCheck(FMOD_ChannelGroup_Set3DMinMaxDistance(channelgroup, mindistance, maxdistance));
    return 0;
}
#endif


/* FMOD_ChannelGroup_Get3DMinMaxDistance(input FMOD_CHANNELGROUP* channelgroup, output float* mindistance, output float* maxdistance, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Get3DMinMaxDistance
#define FMODExt_func_FMOD_ChannelGroup_Get3DMinMaxDistance _FMODExt_func_FMOD_ChannelGroup_Get3DMinMaxDistance
static int _FMODExt_func_FMOD_ChannelGroup_Get3DMinMaxDistance(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float mindistance;
    float maxdistance;
    ensure(LL, FMOD_ChannelGroup_Get3DMinMaxDistance, FMOD_RESULT, FMOD_CHANNELGROUP*, float*, float*);
    errCheck(FMOD_ChannelGroup_Get3DMinMaxDistance(channelgroup, &mindistance, &maxdistance));
    FMODExt_push_float(L, mindistance);
    FMODExt_push_float(L, maxdistance);
    return 2;
}
#endif


/* FMOD_ChannelGroup_Set3DConeSettings(input FMOD_CHANNELGROUP* channelgroup, input float insideconeangle, input float outsideconeangle, input float outsidevolume, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DConeSettings
#define FMODExt_func_FMOD_ChannelGroup_Set3DConeSettings _FMODExt_func_FMOD_ChannelGroup_Set3DConeSettings
static int _FMODExt_func_FMOD_ChannelGroup_Set3DConeSettings(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float insideconeangle = FMODExt_check_float(L, 2);
    float outsideconeangle = FMODExt_check_float(L, 3);
    float outsidevolume = FMODExt_check_float(L, 4);
    ensure(LL, FMOD_ChannelGroup_Set3DConeSettings, FMOD_RESULT, FMOD_CHANNELGROUP*, float, float, float);
    errCheck(FMOD_ChannelGroup_Set3DConeSettings(channelgroup, insideconeangle, outsideconeangle, outsidevolume));
    return 0;
}
#endif


/* FMOD_ChannelGroup_Get3DConeSettings(input FMOD_CHANNELGROUP* channelgroup, output float* insideconeangle, output float* outsideconeangle, output float* outsidevolume, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Get3DConeSettings
#define FMODExt_func_FMOD_ChannelGroup_Get3DConeSettings _FMODExt_func_FMOD_ChannelGroup_Get3DConeSettings
static int _FMODExt_func_FMOD_ChannelGroup_Get3DConeSettings(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float insideconeangle;
    float outsideconeangle;
    float outsidevolume;
    ensure(LL, FMOD_ChannelGroup_Get3DConeSettings, FMOD_RESULT, FMOD_CHANNELGROUP*, float*, float*, float*);
    errCheck(FMOD_ChannelGroup_Get3DConeSettings(channelgroup, &insideconeangle, &outsideconeangle, &outsidevolume));
    FMODExt_push_float(L, insideconeangle);
    FMODExt_push_float(L, outsideconeangle);
    FMODExt_push_float(L, outsidevolume);
    return 3;
}
#endif


/* FMOD_ChannelGroup_Set3DConeOrientation(input FMOD_CHANNELGROUP* channelgroup, output FMOD_VECTOR* orientation, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DConeOrientation
#define FMODExt_func_FMOD_ChannelGroup_Set3DConeOrientation _FMODExt_func_FMOD_ChannelGroup_Set3DConeOrientation
static int _FMODExt_func_FMOD_ChannelGroup_Set3DConeOrientation(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_VECTOR orientation;
    ensure(LL, FMOD_ChannelGroup_Set3DConeOrientation, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_VECTOR*);
    errCheck(FMOD_ChannelGroup_Set3DConeOrientation(channelgroup, &orientation));
    FMODExt_push_FMOD_VECTOR(L, orientation);
    return 1;
}
#endif


/* FMOD_ChannelGroup_Get3DConeOrientation(input FMOD_CHANNELGROUP* channelgroup, output FMOD_VECTOR* orientation, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Get3DConeOrientation
#define FMODExt_func_FMOD_ChannelGroup_Get3DConeOrientation _FMODExt_func_FMOD_ChannelGroup_Get3DConeOrientation
static int _FMODExt_func_FMOD_ChannelGroup_Get3DConeOrientation(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_VECTOR orientation;
    ensure(LL, FMOD_ChannelGroup_Get3DConeOrientation, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_VECTOR*);
    errCheck(FMOD_ChannelGroup_Get3DConeOrientation(channelgroup, &orientation));
    FMODExt_push_FMOD_VECTOR(L, orientation);
    return 1;
}
#endif


/* FMOD_ChannelGroup_Set3DCustomRolloff(input FMOD_CHANNELGROUP* channelgroup, output FMOD_VECTOR* points, input int numpoints, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DCustomRolloff
#define FMODExt_func_FMOD_ChannelGroup_Set3DCustomRolloff _FMODExt_func_FMOD_ChannelGroup_Set3DCustomRolloff
static int _FMODExt_func_FMOD_ChannelGroup_Set3DCustomRolloff(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_VECTOR points;
    int numpoints = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_ChannelGroup_Set3DCustomRolloff, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_VECTOR*, int);
    errCheck(FMOD_ChannelGroup_Set3DCustomRolloff(channelgroup, &points, numpoints));
    FMODExt_push_FMOD_VECTOR(L, points);
    return 1;
}
#endif


/* FMOD_ChannelGroup_Get3DCustomRolloff(input FMOD_CHANNELGROUP* channelgroup, unknown FMOD_VECTOR** points, output int* numpoints, ) */


/* FMOD_ChannelGroup_Set3DOcclusion(input FMOD_CHANNELGROUP* channelgroup, input float directocclusion, input float reverbocclusion, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DOcclusion
#define FMODExt_func_FMOD_ChannelGroup_Set3DOcclusion _FMODExt_func_FMOD_ChannelGroup_Set3DOcclusion
static int _FMODExt_func_FMOD_ChannelGroup_Set3DOcclusion(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float directocclusion = FMODExt_check_float(L, 2);
    float reverbocclusion = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_ChannelGroup_Set3DOcclusion, FMOD_RESULT, FMOD_CHANNELGROUP*, float, float);
    errCheck(FMOD_ChannelGroup_Set3DOcclusion(channelgroup, directocclusion, reverbocclusion));
    return 0;
}
#endif


/* FMOD_ChannelGroup_Get3DOcclusion(input FMOD_CHANNELGROUP* channelgroup, output float* directocclusion, output float* reverbocclusion, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Get3DOcclusion
#define FMODExt_func_FMOD_ChannelGroup_Get3DOcclusion _FMODExt_func_FMOD_ChannelGroup_Get3DOcclusion
static int _FMODExt_func_FMOD_ChannelGroup_Get3DOcclusion(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float directocclusion;
    float reverbocclusion;
    ensure(LL, FMOD_ChannelGroup_Get3DOcclusion, FMOD_RESULT, FMOD_CHANNELGROUP*, float*, float*);
    errCheck(FMOD_ChannelGroup_Get3DOcclusion(channelgroup, &directocclusion, &reverbocclusion));
    FMODExt_push_float(L, directocclusion);
    FMODExt_push_float(L, reverbocclusion);
    return 2;
}
#endif


/* FMOD_ChannelGroup_Set3DSpread(input FMOD_CHANNELGROUP* channelgroup, input float angle, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DSpread
#define FMODExt_func_FMOD_ChannelGroup_Set3DSpread _FMODExt_func_FMOD_ChannelGroup_Set3DSpread
static int _FMODExt_func_FMOD_ChannelGroup_Set3DSpread(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float angle = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_ChannelGroup_Set3DSpread, FMOD_RESULT, FMOD_CHANNELGROUP*, float);
    errCheck(FMOD_ChannelGroup_Set3DSpread(channelgroup, angle));
    return 0;
}
#endif


/* FMOD_ChannelGroup_Get3DSpread(input FMOD_CHANNELGROUP* channelgroup, output float* angle, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Get3DSpread
#define FMODExt_func_FMOD_ChannelGroup_Get3DSpread _FMODExt_func_FMOD_ChannelGroup_Get3DSpread
static int _FMODExt_func_FMOD_ChannelGroup_Get3DSpread(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float angle;
    ensure(LL, FMOD_ChannelGroup_Get3DSpread, FMOD_RESULT, FMOD_CHANNELGROUP*, float*);
    errCheck(FMOD_ChannelGroup_Get3DSpread(channelgroup, &angle));
    FMODExt_push_float(L, angle);
    return 1;
}
#endif


/* FMOD_ChannelGroup_Set3DLevel(input FMOD_CHANNELGROUP* channelgroup, input float level, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DLevel
#define FMODExt_func_FMOD_ChannelGroup_Set3DLevel _FMODExt_func_FMOD_ChannelGroup_Set3DLevel
static int _FMODExt_func_FMOD_ChannelGroup_Set3DLevel(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float level = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_ChannelGroup_Set3DLevel, FMOD_RESULT, FMOD_CHANNELGROUP*, float);
    errCheck(FMOD_ChannelGroup_Set3DLevel(channelgroup, level));
    return 0;
}
#endif


/* FMOD_ChannelGroup_Get3DLevel(input FMOD_CHANNELGROUP* channelgroup, output float* level, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Get3DLevel
#define FMODExt_func_FMOD_ChannelGroup_Get3DLevel _FMODExt_func_FMOD_ChannelGroup_Get3DLevel
static int _FMODExt_func_FMOD_ChannelGroup_Get3DLevel(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float level;
    ensure(LL, FMOD_ChannelGroup_Get3DLevel, FMOD_RESULT, FMOD_CHANNELGROUP*, float*);
    errCheck(FMOD_ChannelGroup_Get3DLevel(channelgroup, &level));
    FMODExt_push_float(L, level);
    return 1;
}
#endif


/* FMOD_ChannelGroup_Set3DDopplerLevel(input FMOD_CHANNELGROUP* channelgroup, input float level, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DDopplerLevel
#define FMODExt_func_FMOD_ChannelGroup_Set3DDopplerLevel _FMODExt_func_FMOD_ChannelGroup_Set3DDopplerLevel
static int _FMODExt_func_FMOD_ChannelGroup_Set3DDopplerLevel(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float level = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_ChannelGroup_Set3DDopplerLevel, FMOD_RESULT, FMOD_CHANNELGROUP*, float);
    errCheck(FMOD_ChannelGroup_Set3DDopplerLevel(channelgroup, level));
    return 0;
}
#endif


/* FMOD_ChannelGroup_Get3DDopplerLevel(input FMOD_CHANNELGROUP* channelgroup, output float* level, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Get3DDopplerLevel
#define FMODExt_func_FMOD_ChannelGroup_Get3DDopplerLevel _FMODExt_func_FMOD_ChannelGroup_Get3DDopplerLevel
static int _FMODExt_func_FMOD_ChannelGroup_Get3DDopplerLevel(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    float level;
    ensure(LL, FMOD_ChannelGroup_Get3DDopplerLevel, FMOD_RESULT, FMOD_CHANNELGROUP*, float*);
    errCheck(FMOD_ChannelGroup_Get3DDopplerLevel(channelgroup, &level));
    FMODExt_push_float(L, level);
    return 1;
}
#endif


/* FMOD_ChannelGroup_Set3DDistanceFilter(input FMOD_CHANNELGROUP* channelgroup, input FMOD_BOOL custom, input float customLevel, input float centerFreq, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Set3DDistanceFilter
#define FMODExt_func_FMOD_ChannelGroup_Set3DDistanceFilter _FMODExt_func_FMOD_ChannelGroup_Set3DDistanceFilter
static int _FMODExt_func_FMOD_ChannelGroup_Set3DDistanceFilter(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_BOOL custom = FMODExt_check_FMOD_BOOL(L, 2);
    float customLevel = FMODExt_check_float(L, 3);
    float centerFreq = FMODExt_check_float(L, 4);
    ensure(LL, FMOD_ChannelGroup_Set3DDistanceFilter, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_BOOL, float, float);
    errCheck(FMOD_ChannelGroup_Set3DDistanceFilter(channelgroup, custom, customLevel, centerFreq));
    return 0;
}
#endif


/* FMOD_ChannelGroup_Get3DDistanceFilter(input FMOD_CHANNELGROUP* channelgroup, output FMOD_BOOL* custom, output float* customLevel, output float* centerFreq, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Get3DDistanceFilter
#define FMODExt_func_FMOD_ChannelGroup_Get3DDistanceFilter _FMODExt_func_FMOD_ChannelGroup_Get3DDistanceFilter
static int _FMODExt_func_FMOD_ChannelGroup_Get3DDistanceFilter(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_BOOL custom;
    float customLevel;
    float centerFreq;
    ensure(LL, FMOD_ChannelGroup_Get3DDistanceFilter, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_BOOL*, float*, float*);
    errCheck(FMOD_ChannelGroup_Get3DDistanceFilter(channelgroup, &custom, &customLevel, &centerFreq));
    FMODExt_push_FMOD_BOOL(L, custom);
    FMODExt_push_float(L, customLevel);
    FMODExt_push_float(L, centerFreq);
    return 3;
}
#endif


/* FMOD_ChannelGroup_SetUserData(input FMOD_CHANNELGROUP* channelgroup, unknown void* userdata, ) */


/* FMOD_ChannelGroup_GetUserData(input FMOD_CHANNELGROUP* channelgroup, unknown void** userdata, ) */


/* FMOD_ChannelGroup_Release(input FMOD_CHANNELGROUP* channelgroup, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_Release
#define FMODExt_func_FMOD_ChannelGroup_Release _FMODExt_func_FMOD_ChannelGroup_Release
static int _FMODExt_func_FMOD_ChannelGroup_Release(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    ensure(LL, FMOD_ChannelGroup_Release, FMOD_RESULT, FMOD_CHANNELGROUP*);
    errCheck(FMOD_ChannelGroup_Release(channelgroup));
    return 0;
}
#endif


/* FMOD_ChannelGroup_AddGroup(input FMOD_CHANNELGROUP* channelgroup, input FMOD_CHANNELGROUP* group, input FMOD_BOOL propagatedspclock, output FMOD_DSPCONNECTION** connection, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_AddGroup
#define FMODExt_func_FMOD_ChannelGroup_AddGroup _FMODExt_func_FMOD_ChannelGroup_AddGroup
static int _FMODExt_func_FMOD_ChannelGroup_AddGroup(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_CHANNELGROUP* group = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 2);
    FMOD_BOOL propagatedspclock = FMODExt_check_FMOD_BOOL(L, 3);
    FMOD_DSPCONNECTION* connection;
    ensure(LL, FMOD_ChannelGroup_AddGroup, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_CHANNELGROUP*, FMOD_BOOL, FMOD_DSPCONNECTION**);
    errCheck(FMOD_ChannelGroup_AddGroup(channelgroup, group, propagatedspclock, &connection));
    FMODExt_push_ptr_FMOD_DSPCONNECTION(L, connection);
    return 1;
}
#endif


/* FMOD_ChannelGroup_GetNumGroups(input FMOD_CHANNELGROUP* channelgroup, output int* numgroups, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetNumGroups
#define FMODExt_func_FMOD_ChannelGroup_GetNumGroups _FMODExt_func_FMOD_ChannelGroup_GetNumGroups
static int _FMODExt_func_FMOD_ChannelGroup_GetNumGroups(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    int numgroups;
    ensure(LL, FMOD_ChannelGroup_GetNumGroups, FMOD_RESULT, FMOD_CHANNELGROUP*, int*);
    errCheck(FMOD_ChannelGroup_GetNumGroups(channelgroup, &numgroups));
    FMODExt_push_int(L, numgroups);
    return 1;
}
#endif


/* FMOD_ChannelGroup_GetGroup(input FMOD_CHANNELGROUP* channelgroup, input int index, output FMOD_CHANNELGROUP** group, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetGroup
#define FMODExt_func_FMOD_ChannelGroup_GetGroup _FMODExt_func_FMOD_ChannelGroup_GetGroup
static int _FMODExt_func_FMOD_ChannelGroup_GetGroup(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_CHANNELGROUP* group;
    ensure(LL, FMOD_ChannelGroup_GetGroup, FMOD_RESULT, FMOD_CHANNELGROUP*, int, FMOD_CHANNELGROUP**);
    errCheck(FMOD_ChannelGroup_GetGroup(channelgroup, index, &group));
    FMODExt_push_ptr_FMOD_CHANNELGROUP(L, group);
    return 1;
}
#endif


/* FMOD_ChannelGroup_GetParentGroup(input FMOD_CHANNELGROUP* channelgroup, output FMOD_CHANNELGROUP** group, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetParentGroup
#define FMODExt_func_FMOD_ChannelGroup_GetParentGroup _FMODExt_func_FMOD_ChannelGroup_GetParentGroup
static int _FMODExt_func_FMOD_ChannelGroup_GetParentGroup(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    FMOD_CHANNELGROUP* group;
    ensure(LL, FMOD_ChannelGroup_GetParentGroup, FMOD_RESULT, FMOD_CHANNELGROUP*, FMOD_CHANNELGROUP**);
    errCheck(FMOD_ChannelGroup_GetParentGroup(channelgroup, &group));
    FMODExt_push_ptr_FMOD_CHANNELGROUP(L, group);
    return 1;
}
#endif


/* FMOD_ChannelGroup_GetName(input FMOD_CHANNELGROUP* channelgroup, unknown char* name, input int namelen, ) */


/* FMOD_ChannelGroup_GetNumChannels(input FMOD_CHANNELGROUP* channelgroup, output int* numchannels, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetNumChannels
#define FMODExt_func_FMOD_ChannelGroup_GetNumChannels _FMODExt_func_FMOD_ChannelGroup_GetNumChannels
static int _FMODExt_func_FMOD_ChannelGroup_GetNumChannels(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    int numchannels;
    ensure(LL, FMOD_ChannelGroup_GetNumChannels, FMOD_RESULT, FMOD_CHANNELGROUP*, int*);
    errCheck(FMOD_ChannelGroup_GetNumChannels(channelgroup, &numchannels));
    FMODExt_push_int(L, numchannels);
    return 1;
}
#endif


/* FMOD_ChannelGroup_GetChannel(input FMOD_CHANNELGROUP* channelgroup, input int index, output FMOD_CHANNEL** channel, ) */
#ifndef FMODExt_func_FMOD_ChannelGroup_GetChannel
#define FMODExt_func_FMOD_ChannelGroup_GetChannel _FMODExt_func_FMOD_ChannelGroup_GetChannel
static int _FMODExt_func_FMOD_ChannelGroup_GetChannel(lua_State *L) {
    FMOD_CHANNELGROUP* channelgroup = FMODExt_check_ptr_FMOD_CHANNELGROUP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_CHANNEL* channel;
    ensure(LL, FMOD_ChannelGroup_GetChannel, FMOD_RESULT, FMOD_CHANNELGROUP*, int, FMOD_CHANNEL**);
    errCheck(FMOD_ChannelGroup_GetChannel(channelgroup, index, &channel));
    FMODExt_push_ptr_FMOD_CHANNEL(L, channel);
    return 1;
}
#endif


/* FMOD_SoundGroup_Release(input FMOD_SOUNDGROUP* soundgroup, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_Release
#define FMODExt_func_FMOD_SoundGroup_Release _FMODExt_func_FMOD_SoundGroup_Release
static int _FMODExt_func_FMOD_SoundGroup_Release(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    ensure(LL, FMOD_SoundGroup_Release, FMOD_RESULT, FMOD_SOUNDGROUP*);
    errCheck(FMOD_SoundGroup_Release(soundgroup));
    return 0;
}
#endif


/* FMOD_SoundGroup_GetSystemObject(input FMOD_SOUNDGROUP* soundgroup, output FMOD_SYSTEM** system, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_GetSystemObject
#define FMODExt_func_FMOD_SoundGroup_GetSystemObject _FMODExt_func_FMOD_SoundGroup_GetSystemObject
static int _FMODExt_func_FMOD_SoundGroup_GetSystemObject(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    FMOD_SYSTEM* system;
    ensure(LL, FMOD_SoundGroup_GetSystemObject, FMOD_RESULT, FMOD_SOUNDGROUP*, FMOD_SYSTEM**);
    errCheck(FMOD_SoundGroup_GetSystemObject(soundgroup, &system));
    FMODExt_push_ptr_FMOD_SYSTEM(L, system);
    return 1;
}
#endif


/* FMOD_SoundGroup_SetMaxAudible(input FMOD_SOUNDGROUP* soundgroup, input int maxaudible, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_SetMaxAudible
#define FMODExt_func_FMOD_SoundGroup_SetMaxAudible _FMODExt_func_FMOD_SoundGroup_SetMaxAudible
static int _FMODExt_func_FMOD_SoundGroup_SetMaxAudible(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    int maxaudible = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_SoundGroup_SetMaxAudible, FMOD_RESULT, FMOD_SOUNDGROUP*, int);
    errCheck(FMOD_SoundGroup_SetMaxAudible(soundgroup, maxaudible));
    return 0;
}
#endif


/* FMOD_SoundGroup_GetMaxAudible(input FMOD_SOUNDGROUP* soundgroup, output int* maxaudible, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_GetMaxAudible
#define FMODExt_func_FMOD_SoundGroup_GetMaxAudible _FMODExt_func_FMOD_SoundGroup_GetMaxAudible
static int _FMODExt_func_FMOD_SoundGroup_GetMaxAudible(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    int maxaudible;
    ensure(LL, FMOD_SoundGroup_GetMaxAudible, FMOD_RESULT, FMOD_SOUNDGROUP*, int*);
    errCheck(FMOD_SoundGroup_GetMaxAudible(soundgroup, &maxaudible));
    FMODExt_push_int(L, maxaudible);
    return 1;
}
#endif


/* FMOD_SoundGroup_SetMaxAudibleBehavior(input FMOD_SOUNDGROUP* soundgroup, input FMOD_SOUNDGROUP_BEHAVIOR behavior, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_SetMaxAudibleBehavior
#define FMODExt_func_FMOD_SoundGroup_SetMaxAudibleBehavior _FMODExt_func_FMOD_SoundGroup_SetMaxAudibleBehavior
static int _FMODExt_func_FMOD_SoundGroup_SetMaxAudibleBehavior(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    FMOD_SOUNDGROUP_BEHAVIOR behavior = FMODExt_check_FMOD_SOUNDGROUP_BEHAVIOR(L, 2);
    ensure(LL, FMOD_SoundGroup_SetMaxAudibleBehavior, FMOD_RESULT, FMOD_SOUNDGROUP*, FMOD_SOUNDGROUP_BEHAVIOR);
    errCheck(FMOD_SoundGroup_SetMaxAudibleBehavior(soundgroup, behavior));
    return 0;
}
#endif


/* FMOD_SoundGroup_GetMaxAudibleBehavior(input FMOD_SOUNDGROUP* soundgroup, output FMOD_SOUNDGROUP_BEHAVIOR* behavior, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_GetMaxAudibleBehavior
#define FMODExt_func_FMOD_SoundGroup_GetMaxAudibleBehavior _FMODExt_func_FMOD_SoundGroup_GetMaxAudibleBehavior
static int _FMODExt_func_FMOD_SoundGroup_GetMaxAudibleBehavior(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    FMOD_SOUNDGROUP_BEHAVIOR behavior;
    ensure(LL, FMOD_SoundGroup_GetMaxAudibleBehavior, FMOD_RESULT, FMOD_SOUNDGROUP*, FMOD_SOUNDGROUP_BEHAVIOR*);
    errCheck(FMOD_SoundGroup_GetMaxAudibleBehavior(soundgroup, &behavior));
    FMODExt_push_FMOD_SOUNDGROUP_BEHAVIOR(L, behavior);
    return 1;
}
#endif


/* FMOD_SoundGroup_SetMuteFadeSpeed(input FMOD_SOUNDGROUP* soundgroup, input float speed, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_SetMuteFadeSpeed
#define FMODExt_func_FMOD_SoundGroup_SetMuteFadeSpeed _FMODExt_func_FMOD_SoundGroup_SetMuteFadeSpeed
static int _FMODExt_func_FMOD_SoundGroup_SetMuteFadeSpeed(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    float speed = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_SoundGroup_SetMuteFadeSpeed, FMOD_RESULT, FMOD_SOUNDGROUP*, float);
    errCheck(FMOD_SoundGroup_SetMuteFadeSpeed(soundgroup, speed));
    return 0;
}
#endif


/* FMOD_SoundGroup_GetMuteFadeSpeed(input FMOD_SOUNDGROUP* soundgroup, output float* speed, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_GetMuteFadeSpeed
#define FMODExt_func_FMOD_SoundGroup_GetMuteFadeSpeed _FMODExt_func_FMOD_SoundGroup_GetMuteFadeSpeed
static int _FMODExt_func_FMOD_SoundGroup_GetMuteFadeSpeed(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    float speed;
    ensure(LL, FMOD_SoundGroup_GetMuteFadeSpeed, FMOD_RESULT, FMOD_SOUNDGROUP*, float*);
    errCheck(FMOD_SoundGroup_GetMuteFadeSpeed(soundgroup, &speed));
    FMODExt_push_float(L, speed);
    return 1;
}
#endif


/* FMOD_SoundGroup_SetVolume(input FMOD_SOUNDGROUP* soundgroup, input float volume, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_SetVolume
#define FMODExt_func_FMOD_SoundGroup_SetVolume _FMODExt_func_FMOD_SoundGroup_SetVolume
static int _FMODExt_func_FMOD_SoundGroup_SetVolume(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    float volume = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_SoundGroup_SetVolume, FMOD_RESULT, FMOD_SOUNDGROUP*, float);
    errCheck(FMOD_SoundGroup_SetVolume(soundgroup, volume));
    return 0;
}
#endif


/* FMOD_SoundGroup_GetVolume(input FMOD_SOUNDGROUP* soundgroup, output float* volume, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_GetVolume
#define FMODExt_func_FMOD_SoundGroup_GetVolume _FMODExt_func_FMOD_SoundGroup_GetVolume
static int _FMODExt_func_FMOD_SoundGroup_GetVolume(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    float volume;
    ensure(LL, FMOD_SoundGroup_GetVolume, FMOD_RESULT, FMOD_SOUNDGROUP*, float*);
    errCheck(FMOD_SoundGroup_GetVolume(soundgroup, &volume));
    FMODExt_push_float(L, volume);
    return 1;
}
#endif


/* FMOD_SoundGroup_Stop(input FMOD_SOUNDGROUP* soundgroup, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_Stop
#define FMODExt_func_FMOD_SoundGroup_Stop _FMODExt_func_FMOD_SoundGroup_Stop
static int _FMODExt_func_FMOD_SoundGroup_Stop(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    ensure(LL, FMOD_SoundGroup_Stop, FMOD_RESULT, FMOD_SOUNDGROUP*);
    errCheck(FMOD_SoundGroup_Stop(soundgroup));
    return 0;
}
#endif


/* FMOD_SoundGroup_GetName(input FMOD_SOUNDGROUP* soundgroup, unknown char* name, input int namelen, ) */


/* FMOD_SoundGroup_GetNumSounds(input FMOD_SOUNDGROUP* soundgroup, output int* numsounds, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_GetNumSounds
#define FMODExt_func_FMOD_SoundGroup_GetNumSounds _FMODExt_func_FMOD_SoundGroup_GetNumSounds
static int _FMODExt_func_FMOD_SoundGroup_GetNumSounds(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    int numsounds;
    ensure(LL, FMOD_SoundGroup_GetNumSounds, FMOD_RESULT, FMOD_SOUNDGROUP*, int*);
    errCheck(FMOD_SoundGroup_GetNumSounds(soundgroup, &numsounds));
    FMODExt_push_int(L, numsounds);
    return 1;
}
#endif


/* FMOD_SoundGroup_GetSound(input FMOD_SOUNDGROUP* soundgroup, input int index, output FMOD_SOUND** sound, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_GetSound
#define FMODExt_func_FMOD_SoundGroup_GetSound _FMODExt_func_FMOD_SoundGroup_GetSound
static int _FMODExt_func_FMOD_SoundGroup_GetSound(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_SOUND* sound;
    ensure(LL, FMOD_SoundGroup_GetSound, FMOD_RESULT, FMOD_SOUNDGROUP*, int, FMOD_SOUND**);
    errCheck(FMOD_SoundGroup_GetSound(soundgroup, index, &sound));
    FMODExt_push_ptr_FMOD_SOUND(L, sound);
    return 1;
}
#endif


/* FMOD_SoundGroup_GetNumPlaying(input FMOD_SOUNDGROUP* soundgroup, output int* numplaying, ) */
#ifndef FMODExt_func_FMOD_SoundGroup_GetNumPlaying
#define FMODExt_func_FMOD_SoundGroup_GetNumPlaying _FMODExt_func_FMOD_SoundGroup_GetNumPlaying
static int _FMODExt_func_FMOD_SoundGroup_GetNumPlaying(lua_State *L) {
    FMOD_SOUNDGROUP* soundgroup = FMODExt_check_ptr_FMOD_SOUNDGROUP(L, 1);
    int numplaying;
    ensure(LL, FMOD_SoundGroup_GetNumPlaying, FMOD_RESULT, FMOD_SOUNDGROUP*, int*);
    errCheck(FMOD_SoundGroup_GetNumPlaying(soundgroup, &numplaying));
    FMODExt_push_int(L, numplaying);
    return 1;
}
#endif


/* FMOD_SoundGroup_SetUserData(input FMOD_SOUNDGROUP* soundgroup, unknown void* userdata, ) */


/* FMOD_SoundGroup_GetUserData(input FMOD_SOUNDGROUP* soundgroup, unknown void** userdata, ) */


/* FMOD_DSP_Release(input FMOD_DSP* dsp, ) */
#ifndef FMODExt_func_FMOD_DSP_Release
#define FMODExt_func_FMOD_DSP_Release _FMODExt_func_FMOD_DSP_Release
static int _FMODExt_func_FMOD_DSP_Release(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    ensure(LL, FMOD_DSP_Release, FMOD_RESULT, FMOD_DSP*);
    errCheck(FMOD_DSP_Release(dsp));
    return 0;
}
#endif


/* FMOD_DSP_GetSystemObject(input FMOD_DSP* dsp, output FMOD_SYSTEM** system, ) */
#ifndef FMODExt_func_FMOD_DSP_GetSystemObject
#define FMODExt_func_FMOD_DSP_GetSystemObject _FMODExt_func_FMOD_DSP_GetSystemObject
static int _FMODExt_func_FMOD_DSP_GetSystemObject(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_SYSTEM* system;
    ensure(LL, FMOD_DSP_GetSystemObject, FMOD_RESULT, FMOD_DSP*, FMOD_SYSTEM**);
    errCheck(FMOD_DSP_GetSystemObject(dsp, &system));
    FMODExt_push_ptr_FMOD_SYSTEM(L, system);
    return 1;
}
#endif


/* FMOD_DSP_AddInput(input FMOD_DSP* dsp, input FMOD_DSP* input, output FMOD_DSPCONNECTION** connection, input FMOD_DSPCONNECTION_TYPE type, ) */
#ifndef FMODExt_func_FMOD_DSP_AddInput
#define FMODExt_func_FMOD_DSP_AddInput _FMODExt_func_FMOD_DSP_AddInput
static int _FMODExt_func_FMOD_DSP_AddInput(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_DSP* input = FMODExt_check_ptr_FMOD_DSP(L, 2);
    FMOD_DSPCONNECTION* connection;
    FMOD_DSPCONNECTION_TYPE type = FMODExt_check_FMOD_DSPCONNECTION_TYPE(L, 3);
    ensure(LL, FMOD_DSP_AddInput, FMOD_RESULT, FMOD_DSP*, FMOD_DSP*, FMOD_DSPCONNECTION**, FMOD_DSPCONNECTION_TYPE);
    errCheck(FMOD_DSP_AddInput(dsp, input, &connection, type));
    FMODExt_push_ptr_FMOD_DSPCONNECTION(L, connection);
    return 1;
}
#endif


/* FMOD_DSP_DisconnectFrom(input FMOD_DSP* dsp, input FMOD_DSP* target, input FMOD_DSPCONNECTION* connection, ) */
#ifndef FMODExt_func_FMOD_DSP_DisconnectFrom
#define FMODExt_func_FMOD_DSP_DisconnectFrom _FMODExt_func_FMOD_DSP_DisconnectFrom
static int _FMODExt_func_FMOD_DSP_DisconnectFrom(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_DSP* target = FMODExt_check_ptr_FMOD_DSP(L, 2);
    FMOD_DSPCONNECTION* connection = FMODExt_check_ptr_FMOD_DSPCONNECTION(L, 3);
    ensure(LL, FMOD_DSP_DisconnectFrom, FMOD_RESULT, FMOD_DSP*, FMOD_DSP*, FMOD_DSPCONNECTION*);
    errCheck(FMOD_DSP_DisconnectFrom(dsp, target, connection));
    return 0;
}
#endif


/* FMOD_DSP_DisconnectAll(input FMOD_DSP* dsp, input FMOD_BOOL inputs, input FMOD_BOOL outputs, ) */
#ifndef FMODExt_func_FMOD_DSP_DisconnectAll
#define FMODExt_func_FMOD_DSP_DisconnectAll _FMODExt_func_FMOD_DSP_DisconnectAll
static int _FMODExt_func_FMOD_DSP_DisconnectAll(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_BOOL inputs = FMODExt_check_FMOD_BOOL(L, 2);
    FMOD_BOOL outputs = FMODExt_check_FMOD_BOOL(L, 3);
    ensure(LL, FMOD_DSP_DisconnectAll, FMOD_RESULT, FMOD_DSP*, FMOD_BOOL, FMOD_BOOL);
    errCheck(FMOD_DSP_DisconnectAll(dsp, inputs, outputs));
    return 0;
}
#endif


/* FMOD_DSP_GetNumInputs(input FMOD_DSP* dsp, output int* numinputs, ) */
#ifndef FMODExt_func_FMOD_DSP_GetNumInputs
#define FMODExt_func_FMOD_DSP_GetNumInputs _FMODExt_func_FMOD_DSP_GetNumInputs
static int _FMODExt_func_FMOD_DSP_GetNumInputs(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int numinputs;
    ensure(LL, FMOD_DSP_GetNumInputs, FMOD_RESULT, FMOD_DSP*, int*);
    errCheck(FMOD_DSP_GetNumInputs(dsp, &numinputs));
    FMODExt_push_int(L, numinputs);
    return 1;
}
#endif


/* FMOD_DSP_GetNumOutputs(input FMOD_DSP* dsp, output int* numoutputs, ) */
#ifndef FMODExt_func_FMOD_DSP_GetNumOutputs
#define FMODExt_func_FMOD_DSP_GetNumOutputs _FMODExt_func_FMOD_DSP_GetNumOutputs
static int _FMODExt_func_FMOD_DSP_GetNumOutputs(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int numoutputs;
    ensure(LL, FMOD_DSP_GetNumOutputs, FMOD_RESULT, FMOD_DSP*, int*);
    errCheck(FMOD_DSP_GetNumOutputs(dsp, &numoutputs));
    FMODExt_push_int(L, numoutputs);
    return 1;
}
#endif


/* FMOD_DSP_GetInput(input FMOD_DSP* dsp, input int index, output FMOD_DSP** input, output FMOD_DSPCONNECTION** inputconnection, ) */
#ifndef FMODExt_func_FMOD_DSP_GetInput
#define FMODExt_func_FMOD_DSP_GetInput _FMODExt_func_FMOD_DSP_GetInput
static int _FMODExt_func_FMOD_DSP_GetInput(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_DSP* input;
    FMOD_DSPCONNECTION* inputconnection;
    ensure(LL, FMOD_DSP_GetInput, FMOD_RESULT, FMOD_DSP*, int, FMOD_DSP**, FMOD_DSPCONNECTION**);
    errCheck(FMOD_DSP_GetInput(dsp, index, &input, &inputconnection));
    FMODExt_push_ptr_FMOD_DSP(L, input);
    FMODExt_push_ptr_FMOD_DSPCONNECTION(L, inputconnection);
    return 2;
}
#endif


/* FMOD_DSP_GetOutput(input FMOD_DSP* dsp, input int index, output FMOD_DSP** output, output FMOD_DSPCONNECTION** outputconnection, ) */
#ifndef FMODExt_func_FMOD_DSP_GetOutput
#define FMODExt_func_FMOD_DSP_GetOutput _FMODExt_func_FMOD_DSP_GetOutput
static int _FMODExt_func_FMOD_DSP_GetOutput(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_DSP* output;
    FMOD_DSPCONNECTION* outputconnection;
    ensure(LL, FMOD_DSP_GetOutput, FMOD_RESULT, FMOD_DSP*, int, FMOD_DSP**, FMOD_DSPCONNECTION**);
    errCheck(FMOD_DSP_GetOutput(dsp, index, &output, &outputconnection));
    FMODExt_push_ptr_FMOD_DSP(L, output);
    FMODExt_push_ptr_FMOD_DSPCONNECTION(L, outputconnection);
    return 2;
}
#endif


/* FMOD_DSP_SetActive(input FMOD_DSP* dsp, input FMOD_BOOL active, ) */
#ifndef FMODExt_func_FMOD_DSP_SetActive
#define FMODExt_func_FMOD_DSP_SetActive _FMODExt_func_FMOD_DSP_SetActive
static int _FMODExt_func_FMOD_DSP_SetActive(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_BOOL active = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_DSP_SetActive, FMOD_RESULT, FMOD_DSP*, FMOD_BOOL);
    errCheck(FMOD_DSP_SetActive(dsp, active));
    return 0;
}
#endif


/* FMOD_DSP_GetActive(input FMOD_DSP* dsp, output FMOD_BOOL* active, ) */
#ifndef FMODExt_func_FMOD_DSP_GetActive
#define FMODExt_func_FMOD_DSP_GetActive _FMODExt_func_FMOD_DSP_GetActive
static int _FMODExt_func_FMOD_DSP_GetActive(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_BOOL active;
    ensure(LL, FMOD_DSP_GetActive, FMOD_RESULT, FMOD_DSP*, FMOD_BOOL*);
    errCheck(FMOD_DSP_GetActive(dsp, &active));
    FMODExt_push_FMOD_BOOL(L, active);
    return 1;
}
#endif


/* FMOD_DSP_SetBypass(input FMOD_DSP* dsp, input FMOD_BOOL bypass, ) */
#ifndef FMODExt_func_FMOD_DSP_SetBypass
#define FMODExt_func_FMOD_DSP_SetBypass _FMODExt_func_FMOD_DSP_SetBypass
static int _FMODExt_func_FMOD_DSP_SetBypass(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_BOOL bypass = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_DSP_SetBypass, FMOD_RESULT, FMOD_DSP*, FMOD_BOOL);
    errCheck(FMOD_DSP_SetBypass(dsp, bypass));
    return 0;
}
#endif


/* FMOD_DSP_GetBypass(input FMOD_DSP* dsp, output FMOD_BOOL* bypass, ) */
#ifndef FMODExt_func_FMOD_DSP_GetBypass
#define FMODExt_func_FMOD_DSP_GetBypass _FMODExt_func_FMOD_DSP_GetBypass
static int _FMODExt_func_FMOD_DSP_GetBypass(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_BOOL bypass;
    ensure(LL, FMOD_DSP_GetBypass, FMOD_RESULT, FMOD_DSP*, FMOD_BOOL*);
    errCheck(FMOD_DSP_GetBypass(dsp, &bypass));
    FMODExt_push_FMOD_BOOL(L, bypass);
    return 1;
}
#endif


/* FMOD_DSP_SetWetDryMix(input FMOD_DSP* dsp, input float prewet, input float postwet, input float dry, ) */
#ifndef FMODExt_func_FMOD_DSP_SetWetDryMix
#define FMODExt_func_FMOD_DSP_SetWetDryMix _FMODExt_func_FMOD_DSP_SetWetDryMix
static int _FMODExt_func_FMOD_DSP_SetWetDryMix(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    float prewet = FMODExt_check_float(L, 2);
    float postwet = FMODExt_check_float(L, 3);
    float dry = FMODExt_check_float(L, 4);
    ensure(LL, FMOD_DSP_SetWetDryMix, FMOD_RESULT, FMOD_DSP*, float, float, float);
    errCheck(FMOD_DSP_SetWetDryMix(dsp, prewet, postwet, dry));
    return 0;
}
#endif


/* FMOD_DSP_GetWetDryMix(input FMOD_DSP* dsp, output float* prewet, output float* postwet, output float* dry, ) */
#ifndef FMODExt_func_FMOD_DSP_GetWetDryMix
#define FMODExt_func_FMOD_DSP_GetWetDryMix _FMODExt_func_FMOD_DSP_GetWetDryMix
static int _FMODExt_func_FMOD_DSP_GetWetDryMix(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    float prewet;
    float postwet;
    float dry;
    ensure(LL, FMOD_DSP_GetWetDryMix, FMOD_RESULT, FMOD_DSP*, float*, float*, float*);
    errCheck(FMOD_DSP_GetWetDryMix(dsp, &prewet, &postwet, &dry));
    FMODExt_push_float(L, prewet);
    FMODExt_push_float(L, postwet);
    FMODExt_push_float(L, dry);
    return 3;
}
#endif


/* FMOD_DSP_SetChannelFormat(input FMOD_DSP* dsp, input FMOD_CHANNELMASK channelmask, input int numchannels, input FMOD_SPEAKERMODE source_speakermode, ) */
#ifndef FMODExt_func_FMOD_DSP_SetChannelFormat
#define FMODExt_func_FMOD_DSP_SetChannelFormat _FMODExt_func_FMOD_DSP_SetChannelFormat
static int _FMODExt_func_FMOD_DSP_SetChannelFormat(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_CHANNELMASK channelmask = FMODExt_check_unsigned_int(L, 2);
    int numchannels = FMODExt_check_int(L, 3);
    FMOD_SPEAKERMODE source_speakermode = FMODExt_check_FMOD_SPEAKERMODE(L, 4);
    ensure(LL, FMOD_DSP_SetChannelFormat, FMOD_RESULT, FMOD_DSP*, FMOD_CHANNELMASK, int, FMOD_SPEAKERMODE);
    errCheck(FMOD_DSP_SetChannelFormat(dsp, channelmask, numchannels, source_speakermode));
    return 0;
}
#endif


/* FMOD_DSP_GetChannelFormat(input FMOD_DSP* dsp, output FMOD_CHANNELMASK* channelmask, output int* numchannels, output FMOD_SPEAKERMODE* source_speakermode, ) */
#ifndef FMODExt_func_FMOD_DSP_GetChannelFormat
#define FMODExt_func_FMOD_DSP_GetChannelFormat _FMODExt_func_FMOD_DSP_GetChannelFormat
static int _FMODExt_func_FMOD_DSP_GetChannelFormat(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_CHANNELMASK channelmask;
    int numchannels;
    FMOD_SPEAKERMODE source_speakermode;
    ensure(LL, FMOD_DSP_GetChannelFormat, FMOD_RESULT, FMOD_DSP*, FMOD_CHANNELMASK*, int*, FMOD_SPEAKERMODE*);
    errCheck(FMOD_DSP_GetChannelFormat(dsp, &channelmask, &numchannels, &source_speakermode));
    FMODExt_push_unsigned_int(L, channelmask);
    FMODExt_push_int(L, numchannels);
    FMODExt_push_FMOD_SPEAKERMODE(L, source_speakermode);
    return 3;
}
#endif


/* FMOD_DSP_GetOutputChannelFormat(input FMOD_DSP* dsp, input FMOD_CHANNELMASK inmask, input int inchannels, input FMOD_SPEAKERMODE inspeakermode, output FMOD_CHANNELMASK* outmask, output int* outchannels, output FMOD_SPEAKERMODE* outspeakermode, ) */
#ifndef FMODExt_func_FMOD_DSP_GetOutputChannelFormat
#define FMODExt_func_FMOD_DSP_GetOutputChannelFormat _FMODExt_func_FMOD_DSP_GetOutputChannelFormat
static int _FMODExt_func_FMOD_DSP_GetOutputChannelFormat(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_CHANNELMASK inmask = FMODExt_check_unsigned_int(L, 2);
    int inchannels = FMODExt_check_int(L, 3);
    FMOD_SPEAKERMODE inspeakermode = FMODExt_check_FMOD_SPEAKERMODE(L, 4);
    FMOD_CHANNELMASK outmask;
    int outchannels;
    FMOD_SPEAKERMODE outspeakermode;
    ensure(LL, FMOD_DSP_GetOutputChannelFormat, FMOD_RESULT, FMOD_DSP*, FMOD_CHANNELMASK, int, FMOD_SPEAKERMODE, FMOD_CHANNELMASK*, int*, FMOD_SPEAKERMODE*);
    errCheck(FMOD_DSP_GetOutputChannelFormat(dsp, inmask, inchannels, inspeakermode, &outmask, &outchannels, &outspeakermode));
    FMODExt_push_unsigned_int(L, outmask);
    FMODExt_push_int(L, outchannels);
    FMODExt_push_FMOD_SPEAKERMODE(L, outspeakermode);
    return 3;
}
#endif


/* FMOD_DSP_Reset(input FMOD_DSP* dsp, ) */
#ifndef FMODExt_func_FMOD_DSP_Reset
#define FMODExt_func_FMOD_DSP_Reset _FMODExt_func_FMOD_DSP_Reset
static int _FMODExt_func_FMOD_DSP_Reset(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    ensure(LL, FMOD_DSP_Reset, FMOD_RESULT, FMOD_DSP*);
    errCheck(FMOD_DSP_Reset(dsp));
    return 0;
}
#endif


/* FMOD_DSP_SetCallback(input FMOD_DSP* dsp, unknown FMOD_DSP_CALLBACK callback, ) */


/* FMOD_DSP_SetParameterFloat(input FMOD_DSP* dsp, input int index, input float value, ) */
#ifndef FMODExt_func_FMOD_DSP_SetParameterFloat
#define FMODExt_func_FMOD_DSP_SetParameterFloat _FMODExt_func_FMOD_DSP_SetParameterFloat
static int _FMODExt_func_FMOD_DSP_SetParameterFloat(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    float value = FMODExt_check_float(L, 3);
    ensure(LL, FMOD_DSP_SetParameterFloat, FMOD_RESULT, FMOD_DSP*, int, float);
    errCheck(FMOD_DSP_SetParameterFloat(dsp, index, value));
    return 0;
}
#endif


/* FMOD_DSP_SetParameterInt(input FMOD_DSP* dsp, input int index, input int value, ) */
#ifndef FMODExt_func_FMOD_DSP_SetParameterInt
#define FMODExt_func_FMOD_DSP_SetParameterInt _FMODExt_func_FMOD_DSP_SetParameterInt
static int _FMODExt_func_FMOD_DSP_SetParameterInt(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    int value = FMODExt_check_int(L, 3);
    ensure(LL, FMOD_DSP_SetParameterInt, FMOD_RESULT, FMOD_DSP*, int, int);
    errCheck(FMOD_DSP_SetParameterInt(dsp, index, value));
    return 0;
}
#endif


/* FMOD_DSP_SetParameterBool(input FMOD_DSP* dsp, input int index, input FMOD_BOOL value, ) */
#ifndef FMODExt_func_FMOD_DSP_SetParameterBool
#define FMODExt_func_FMOD_DSP_SetParameterBool _FMODExt_func_FMOD_DSP_SetParameterBool
static int _FMODExt_func_FMOD_DSP_SetParameterBool(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_BOOL value = FMODExt_check_FMOD_BOOL(L, 3);
    ensure(LL, FMOD_DSP_SetParameterBool, FMOD_RESULT, FMOD_DSP*, int, FMOD_BOOL);
    errCheck(FMOD_DSP_SetParameterBool(dsp, index, value));
    return 0;
}
#endif


/* FMOD_DSP_SetParameterData(input FMOD_DSP* dsp, input int index, unknown void* data, input unsigned int length, ) */


/* FMOD_DSP_GetParameterFloat(input FMOD_DSP* dsp, input int index, output float* value, unknown char* valuestr, input int valuestrlen, ) */


/* FMOD_DSP_GetParameterInt(input FMOD_DSP* dsp, input int index, output int* value, unknown char* valuestr, input int valuestrlen, ) */


/* FMOD_DSP_GetParameterBool(input FMOD_DSP* dsp, input int index, output FMOD_BOOL* value, unknown char* valuestr, input int valuestrlen, ) */


/* FMOD_DSP_GetParameterData(input FMOD_DSP* dsp, input int index, unknown void** data, output unsigned int* length, unknown char* valuestr, input int valuestrlen, ) */


/* FMOD_DSP_GetNumParameters(input FMOD_DSP* dsp, output int* numparams, ) */
#ifndef FMODExt_func_FMOD_DSP_GetNumParameters
#define FMODExt_func_FMOD_DSP_GetNumParameters _FMODExt_func_FMOD_DSP_GetNumParameters
static int _FMODExt_func_FMOD_DSP_GetNumParameters(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int numparams;
    ensure(LL, FMOD_DSP_GetNumParameters, FMOD_RESULT, FMOD_DSP*, int*);
    errCheck(FMOD_DSP_GetNumParameters(dsp, &numparams));
    FMODExt_push_int(L, numparams);
    return 1;
}
#endif


/* FMOD_DSP_GetParameterInfo(input FMOD_DSP* dsp, input int index, output FMOD_DSP_PARAMETER_DESC** desc, ) */
#ifndef FMODExt_func_FMOD_DSP_GetParameterInfo
#define FMODExt_func_FMOD_DSP_GetParameterInfo _FMODExt_func_FMOD_DSP_GetParameterInfo
static int _FMODExt_func_FMOD_DSP_GetParameterInfo(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_DSP_PARAMETER_DESC* desc;
    ensure(LL, FMOD_DSP_GetParameterInfo, FMOD_RESULT, FMOD_DSP*, int, FMOD_DSP_PARAMETER_DESC**);
    errCheck(FMOD_DSP_GetParameterInfo(dsp, index, &desc));
    FMODExt_push_ptr_FMOD_DSP_PARAMETER_DESC(L, desc);
    return 1;
}
#endif


/* FMOD_DSP_GetDataParameterIndex(input FMOD_DSP* dsp, input int datatype, output int* index, ) */
#ifndef FMODExt_func_FMOD_DSP_GetDataParameterIndex
#define FMODExt_func_FMOD_DSP_GetDataParameterIndex _FMODExt_func_FMOD_DSP_GetDataParameterIndex
static int _FMODExt_func_FMOD_DSP_GetDataParameterIndex(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    int datatype = FMODExt_check_int(L, 2);
    int index;
    ensure(LL, FMOD_DSP_GetDataParameterIndex, FMOD_RESULT, FMOD_DSP*, int, int*);
    errCheck(FMOD_DSP_GetDataParameterIndex(dsp, datatype, &index));
    FMODExt_push_int(L, index);
    return 1;
}
#endif


/* FMOD_DSP_ShowConfigDialog(input FMOD_DSP* dsp, unknown void* hwnd, input FMOD_BOOL show, ) */


/* FMOD_DSP_GetInfo(input FMOD_DSP* dsp, unknown char* name, output unsigned int* version, output int* channels, output int* configwidth, output int* configheight, ) */


/* FMOD_DSP_GetType(input FMOD_DSP* dsp, output FMOD_DSP_TYPE* type, ) */
#ifndef FMODExt_func_FMOD_DSP_GetType
#define FMODExt_func_FMOD_DSP_GetType _FMODExt_func_FMOD_DSP_GetType
static int _FMODExt_func_FMOD_DSP_GetType(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_DSP_TYPE type;
    ensure(LL, FMOD_DSP_GetType, FMOD_RESULT, FMOD_DSP*, FMOD_DSP_TYPE*);
    errCheck(FMOD_DSP_GetType(dsp, &type));
    FMODExt_push_FMOD_DSP_TYPE(L, type);
    return 1;
}
#endif


/* FMOD_DSP_GetIdle(input FMOD_DSP* dsp, output FMOD_BOOL* idle, ) */
#ifndef FMODExt_func_FMOD_DSP_GetIdle
#define FMODExt_func_FMOD_DSP_GetIdle _FMODExt_func_FMOD_DSP_GetIdle
static int _FMODExt_func_FMOD_DSP_GetIdle(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_BOOL idle;
    ensure(LL, FMOD_DSP_GetIdle, FMOD_RESULT, FMOD_DSP*, FMOD_BOOL*);
    errCheck(FMOD_DSP_GetIdle(dsp, &idle));
    FMODExt_push_FMOD_BOOL(L, idle);
    return 1;
}
#endif


/* FMOD_DSP_SetUserData(input FMOD_DSP* dsp, unknown void* userdata, ) */


/* FMOD_DSP_GetUserData(input FMOD_DSP* dsp, unknown void** userdata, ) */


/* FMOD_DSP_SetMeteringEnabled(input FMOD_DSP* dsp, input FMOD_BOOL inputEnabled, input FMOD_BOOL outputEnabled, ) */
#ifndef FMODExt_func_FMOD_DSP_SetMeteringEnabled
#define FMODExt_func_FMOD_DSP_SetMeteringEnabled _FMODExt_func_FMOD_DSP_SetMeteringEnabled
static int _FMODExt_func_FMOD_DSP_SetMeteringEnabled(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_BOOL inputEnabled = FMODExt_check_FMOD_BOOL(L, 2);
    FMOD_BOOL outputEnabled = FMODExt_check_FMOD_BOOL(L, 3);
    ensure(LL, FMOD_DSP_SetMeteringEnabled, FMOD_RESULT, FMOD_DSP*, FMOD_BOOL, FMOD_BOOL);
    errCheck(FMOD_DSP_SetMeteringEnabled(dsp, inputEnabled, outputEnabled));
    return 0;
}
#endif


/* FMOD_DSP_GetMeteringEnabled(input FMOD_DSP* dsp, output FMOD_BOOL* inputEnabled, output FMOD_BOOL* outputEnabled, ) */
#ifndef FMODExt_func_FMOD_DSP_GetMeteringEnabled
#define FMODExt_func_FMOD_DSP_GetMeteringEnabled _FMODExt_func_FMOD_DSP_GetMeteringEnabled
static int _FMODExt_func_FMOD_DSP_GetMeteringEnabled(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_BOOL inputEnabled;
    FMOD_BOOL outputEnabled;
    ensure(LL, FMOD_DSP_GetMeteringEnabled, FMOD_RESULT, FMOD_DSP*, FMOD_BOOL*, FMOD_BOOL*);
    errCheck(FMOD_DSP_GetMeteringEnabled(dsp, &inputEnabled, &outputEnabled));
    FMODExt_push_FMOD_BOOL(L, inputEnabled);
    FMODExt_push_FMOD_BOOL(L, outputEnabled);
    return 2;
}
#endif


/* FMOD_DSP_GetMeteringInfo(input FMOD_DSP* dsp, output_ptr FMOD_DSP_METERING_INFO* inputInfo, output_ptr FMOD_DSP_METERING_INFO* outputInfo, ) */
#ifndef FMODExt_func_FMOD_DSP_GetMeteringInfo
#define FMODExt_func_FMOD_DSP_GetMeteringInfo _FMODExt_func_FMOD_DSP_GetMeteringInfo
static int _FMODExt_func_FMOD_DSP_GetMeteringInfo(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    FMOD_DSP_METERING_INFO* inputInfo = FMODExt_push_ptr_FMOD_DSP_METERING_INFO(L, NULL);
    FMOD_DSP_METERING_INFO* outputInfo = FMODExt_push_ptr_FMOD_DSP_METERING_INFO(L, NULL);
    ensure(LL, FMOD_DSP_GetMeteringInfo, FMOD_RESULT, FMOD_DSP*, FMOD_DSP_METERING_INFO*, FMOD_DSP_METERING_INFO*);
    errCheck(FMOD_DSP_GetMeteringInfo(dsp, inputInfo, outputInfo));
    lua_pushvalue(L, -2);
    lua_pushvalue(L, -2);
    return 2;
}
#endif


/* FMOD_DSP_GetCPUUsage(input FMOD_DSP* dsp, output unsigned int* exclusive, output unsigned int* inclusive, ) */
#ifndef FMODExt_func_FMOD_DSP_GetCPUUsage
#define FMODExt_func_FMOD_DSP_GetCPUUsage _FMODExt_func_FMOD_DSP_GetCPUUsage
static int _FMODExt_func_FMOD_DSP_GetCPUUsage(lua_State *L) {
    FMOD_DSP* dsp = FMODExt_check_ptr_FMOD_DSP(L, 1);
    unsigned int exclusive;
    unsigned int inclusive;
    ensure(LL, FMOD_DSP_GetCPUUsage, FMOD_RESULT, FMOD_DSP*, unsigned int*, unsigned int*);
    errCheck(FMOD_DSP_GetCPUUsage(dsp, &exclusive, &inclusive));
    FMODExt_push_unsigned_int(L, exclusive);
    FMODExt_push_unsigned_int(L, inclusive);
    return 2;
}
#endif


/* FMOD_DSPConnection_GetInput(input FMOD_DSPCONNECTION* dspconnection, output FMOD_DSP** input, ) */
#ifndef FMODExt_func_FMOD_DSPConnection_GetInput
#define FMODExt_func_FMOD_DSPConnection_GetInput _FMODExt_func_FMOD_DSPConnection_GetInput
static int _FMODExt_func_FMOD_DSPConnection_GetInput(lua_State *L) {
    FMOD_DSPCONNECTION* dspconnection = FMODExt_check_ptr_FMOD_DSPCONNECTION(L, 1);
    FMOD_DSP* input;
    ensure(LL, FMOD_DSPConnection_GetInput, FMOD_RESULT, FMOD_DSPCONNECTION*, FMOD_DSP**);
    errCheck(FMOD_DSPConnection_GetInput(dspconnection, &input));
    FMODExt_push_ptr_FMOD_DSP(L, input);
    return 1;
}
#endif


/* FMOD_DSPConnection_GetOutput(input FMOD_DSPCONNECTION* dspconnection, output FMOD_DSP** output, ) */
#ifndef FMODExt_func_FMOD_DSPConnection_GetOutput
#define FMODExt_func_FMOD_DSPConnection_GetOutput _FMODExt_func_FMOD_DSPConnection_GetOutput
static int _FMODExt_func_FMOD_DSPConnection_GetOutput(lua_State *L) {
    FMOD_DSPCONNECTION* dspconnection = FMODExt_check_ptr_FMOD_DSPCONNECTION(L, 1);
    FMOD_DSP* output;
    ensure(LL, FMOD_DSPConnection_GetOutput, FMOD_RESULT, FMOD_DSPCONNECTION*, FMOD_DSP**);
    errCheck(FMOD_DSPConnection_GetOutput(dspconnection, &output));
    FMODExt_push_ptr_FMOD_DSP(L, output);
    return 1;
}
#endif


/* FMOD_DSPConnection_SetMix(input FMOD_DSPCONNECTION* dspconnection, input float volume, ) */
#ifndef FMODExt_func_FMOD_DSPConnection_SetMix
#define FMODExt_func_FMOD_DSPConnection_SetMix _FMODExt_func_FMOD_DSPConnection_SetMix
static int _FMODExt_func_FMOD_DSPConnection_SetMix(lua_State *L) {
    FMOD_DSPCONNECTION* dspconnection = FMODExt_check_ptr_FMOD_DSPCONNECTION(L, 1);
    float volume = FMODExt_check_float(L, 2);
    ensure(LL, FMOD_DSPConnection_SetMix, FMOD_RESULT, FMOD_DSPCONNECTION*, float);
    errCheck(FMOD_DSPConnection_SetMix(dspconnection, volume));
    return 0;
}
#endif


/* FMOD_DSPConnection_GetMix(input FMOD_DSPCONNECTION* dspconnection, output float* volume, ) */
#ifndef FMODExt_func_FMOD_DSPConnection_GetMix
#define FMODExt_func_FMOD_DSPConnection_GetMix _FMODExt_func_FMOD_DSPConnection_GetMix
static int _FMODExt_func_FMOD_DSPConnection_GetMix(lua_State *L) {
    FMOD_DSPCONNECTION* dspconnection = FMODExt_check_ptr_FMOD_DSPCONNECTION(L, 1);
    float volume;
    ensure(LL, FMOD_DSPConnection_GetMix, FMOD_RESULT, FMOD_DSPCONNECTION*, float*);
    errCheck(FMOD_DSPConnection_GetMix(dspconnection, &volume));
    FMODExt_push_float(L, volume);
    return 1;
}
#endif


/* FMOD_DSPConnection_SetMixMatrix(input FMOD_DSPCONNECTION* dspconnection, output float* matrix, input int outchannels, input int inchannels, input int inchannel_hop, ) */
#ifndef FMODExt_func_FMOD_DSPConnection_SetMixMatrix
#define FMODExt_func_FMOD_DSPConnection_SetMixMatrix _FMODExt_func_FMOD_DSPConnection_SetMixMatrix
static int _FMODExt_func_FMOD_DSPConnection_SetMixMatrix(lua_State *L) {
    FMOD_DSPCONNECTION* dspconnection = FMODExt_check_ptr_FMOD_DSPCONNECTION(L, 1);
    float matrix;
    int outchannels = FMODExt_check_int(L, 2);
    int inchannels = FMODExt_check_int(L, 3);
    int inchannel_hop = FMODExt_check_int(L, 4);
    ensure(LL, FMOD_DSPConnection_SetMixMatrix, FMOD_RESULT, FMOD_DSPCONNECTION*, float*, int, int, int);
    errCheck(FMOD_DSPConnection_SetMixMatrix(dspconnection, &matrix, outchannels, inchannels, inchannel_hop));
    FMODExt_push_float(L, matrix);
    return 1;
}
#endif


/* FMOD_DSPConnection_GetMixMatrix(input FMOD_DSPCONNECTION* dspconnection, output float* matrix, output int* outchannels, output int* inchannels, input int inchannel_hop, ) */
#ifndef FMODExt_func_FMOD_DSPConnection_GetMixMatrix
#define FMODExt_func_FMOD_DSPConnection_GetMixMatrix _FMODExt_func_FMOD_DSPConnection_GetMixMatrix
static int _FMODExt_func_FMOD_DSPConnection_GetMixMatrix(lua_State *L) {
    FMOD_DSPCONNECTION* dspconnection = FMODExt_check_ptr_FMOD_DSPCONNECTION(L, 1);
    float matrix;
    int outchannels;
    int inchannels;
    int inchannel_hop = FMODExt_check_int(L, 2);
    ensure(LL, FMOD_DSPConnection_GetMixMatrix, FMOD_RESULT, FMOD_DSPCONNECTION*, float*, int*, int*, int);
    errCheck(FMOD_DSPConnection_GetMixMatrix(dspconnection, &matrix, &outchannels, &inchannels, inchannel_hop));
    FMODExt_push_float(L, matrix);
    FMODExt_push_int(L, outchannels);
    FMODExt_push_int(L, inchannels);
    return 3;
}
#endif


/* FMOD_DSPConnection_GetType(input FMOD_DSPCONNECTION* dspconnection, output FMOD_DSPCONNECTION_TYPE* type, ) */
#ifndef FMODExt_func_FMOD_DSPConnection_GetType
#define FMODExt_func_FMOD_DSPConnection_GetType _FMODExt_func_FMOD_DSPConnection_GetType
static int _FMODExt_func_FMOD_DSPConnection_GetType(lua_State *L) {
    FMOD_DSPCONNECTION* dspconnection = FMODExt_check_ptr_FMOD_DSPCONNECTION(L, 1);
    FMOD_DSPCONNECTION_TYPE type;
    ensure(LL, FMOD_DSPConnection_GetType, FMOD_RESULT, FMOD_DSPCONNECTION*, FMOD_DSPCONNECTION_TYPE*);
    errCheck(FMOD_DSPConnection_GetType(dspconnection, &type));
    FMODExt_push_FMOD_DSPCONNECTION_TYPE(L, type);
    return 1;
}
#endif


/* FMOD_DSPConnection_SetUserData(input FMOD_DSPCONNECTION* dspconnection, unknown void* userdata, ) */


/* FMOD_DSPConnection_GetUserData(input FMOD_DSPCONNECTION* dspconnection, unknown void** userdata, ) */


/* FMOD_Geometry_Release(input FMOD_GEOMETRY* geometry, ) */
#ifndef FMODExt_func_FMOD_Geometry_Release
#define FMODExt_func_FMOD_Geometry_Release _FMODExt_func_FMOD_Geometry_Release
static int _FMODExt_func_FMOD_Geometry_Release(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    ensure(LL, FMOD_Geometry_Release, FMOD_RESULT, FMOD_GEOMETRY*);
    errCheck(FMOD_Geometry_Release(geometry));
    return 0;
}
#endif


/* FMOD_Geometry_AddPolygon(input FMOD_GEOMETRY* geometry, input float directocclusion, input float reverbocclusion, input FMOD_BOOL doublesided, input int numvertices, input_ptr const FMOD_VECTOR* vertices, output int* polygonindex, ) */
#ifndef FMODExt_func_FMOD_Geometry_AddPolygon
#define FMODExt_func_FMOD_Geometry_AddPolygon _FMODExt_func_FMOD_Geometry_AddPolygon
static int _FMODExt_func_FMOD_Geometry_AddPolygon(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    float directocclusion = FMODExt_check_float(L, 2);
    float reverbocclusion = FMODExt_check_float(L, 3);
    FMOD_BOOL doublesided = FMODExt_check_FMOD_BOOL(L, 4);
    int numvertices = FMODExt_check_int(L, 5);
    const FMOD_VECTOR vertices = FMODExt_check_FMOD_VECTOR(L, 6);
    int polygonindex;
    ensure(LL, FMOD_Geometry_AddPolygon, FMOD_RESULT, FMOD_GEOMETRY*, float, float, FMOD_BOOL, int, const FMOD_VECTOR*, int*);
    errCheck(FMOD_Geometry_AddPolygon(geometry, directocclusion, reverbocclusion, doublesided, numvertices, &vertices, &polygonindex));
    FMODExt_push_int(L, polygonindex);
    return 1;
}
#endif


/* FMOD_Geometry_GetNumPolygons(input FMOD_GEOMETRY* geometry, output int* numpolygons, ) */
#ifndef FMODExt_func_FMOD_Geometry_GetNumPolygons
#define FMODExt_func_FMOD_Geometry_GetNumPolygons _FMODExt_func_FMOD_Geometry_GetNumPolygons
static int _FMODExt_func_FMOD_Geometry_GetNumPolygons(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    int numpolygons;
    ensure(LL, FMOD_Geometry_GetNumPolygons, FMOD_RESULT, FMOD_GEOMETRY*, int*);
    errCheck(FMOD_Geometry_GetNumPolygons(geometry, &numpolygons));
    FMODExt_push_int(L, numpolygons);
    return 1;
}
#endif


/* FMOD_Geometry_GetMaxPolygons(input FMOD_GEOMETRY* geometry, output int* maxpolygons, output int* maxvertices, ) */
#ifndef FMODExt_func_FMOD_Geometry_GetMaxPolygons
#define FMODExt_func_FMOD_Geometry_GetMaxPolygons _FMODExt_func_FMOD_Geometry_GetMaxPolygons
static int _FMODExt_func_FMOD_Geometry_GetMaxPolygons(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    int maxpolygons;
    int maxvertices;
    ensure(LL, FMOD_Geometry_GetMaxPolygons, FMOD_RESULT, FMOD_GEOMETRY*, int*, int*);
    errCheck(FMOD_Geometry_GetMaxPolygons(geometry, &maxpolygons, &maxvertices));
    FMODExt_push_int(L, maxpolygons);
    FMODExt_push_int(L, maxvertices);
    return 2;
}
#endif


/* FMOD_Geometry_GetPolygonNumVertices(input FMOD_GEOMETRY* geometry, input int index, output int* numvertices, ) */
#ifndef FMODExt_func_FMOD_Geometry_GetPolygonNumVertices
#define FMODExt_func_FMOD_Geometry_GetPolygonNumVertices _FMODExt_func_FMOD_Geometry_GetPolygonNumVertices
static int _FMODExt_func_FMOD_Geometry_GetPolygonNumVertices(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    int index = FMODExt_check_int(L, 2);
    int numvertices;
    ensure(LL, FMOD_Geometry_GetPolygonNumVertices, FMOD_RESULT, FMOD_GEOMETRY*, int, int*);
    errCheck(FMOD_Geometry_GetPolygonNumVertices(geometry, index, &numvertices));
    FMODExt_push_int(L, numvertices);
    return 1;
}
#endif


/* FMOD_Geometry_SetPolygonVertex(input FMOD_GEOMETRY* geometry, input int index, input int vertexindex, input_ptr const FMOD_VECTOR* vertex, ) */
#ifndef FMODExt_func_FMOD_Geometry_SetPolygonVertex
#define FMODExt_func_FMOD_Geometry_SetPolygonVertex _FMODExt_func_FMOD_Geometry_SetPolygonVertex
static int _FMODExt_func_FMOD_Geometry_SetPolygonVertex(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    int index = FMODExt_check_int(L, 2);
    int vertexindex = FMODExt_check_int(L, 3);
    const FMOD_VECTOR vertex = FMODExt_check_FMOD_VECTOR(L, 4);
    ensure(LL, FMOD_Geometry_SetPolygonVertex, FMOD_RESULT, FMOD_GEOMETRY*, int, int, const FMOD_VECTOR*);
    errCheck(FMOD_Geometry_SetPolygonVertex(geometry, index, vertexindex, &vertex));
    return 0;
}
#endif


/* FMOD_Geometry_GetPolygonVertex(input FMOD_GEOMETRY* geometry, input int index, input int vertexindex, output FMOD_VECTOR* vertex, ) */
#ifndef FMODExt_func_FMOD_Geometry_GetPolygonVertex
#define FMODExt_func_FMOD_Geometry_GetPolygonVertex _FMODExt_func_FMOD_Geometry_GetPolygonVertex
static int _FMODExt_func_FMOD_Geometry_GetPolygonVertex(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    int index = FMODExt_check_int(L, 2);
    int vertexindex = FMODExt_check_int(L, 3);
    FMOD_VECTOR vertex;
    ensure(LL, FMOD_Geometry_GetPolygonVertex, FMOD_RESULT, FMOD_GEOMETRY*, int, int, FMOD_VECTOR*);
    errCheck(FMOD_Geometry_GetPolygonVertex(geometry, index, vertexindex, &vertex));
    FMODExt_push_FMOD_VECTOR(L, vertex);
    return 1;
}
#endif


/* FMOD_Geometry_SetPolygonAttributes(input FMOD_GEOMETRY* geometry, input int index, input float directocclusion, input float reverbocclusion, input FMOD_BOOL doublesided, ) */
#ifndef FMODExt_func_FMOD_Geometry_SetPolygonAttributes
#define FMODExt_func_FMOD_Geometry_SetPolygonAttributes _FMODExt_func_FMOD_Geometry_SetPolygonAttributes
static int _FMODExt_func_FMOD_Geometry_SetPolygonAttributes(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    int index = FMODExt_check_int(L, 2);
    float directocclusion = FMODExt_check_float(L, 3);
    float reverbocclusion = FMODExt_check_float(L, 4);
    FMOD_BOOL doublesided = FMODExt_check_FMOD_BOOL(L, 5);
    ensure(LL, FMOD_Geometry_SetPolygonAttributes, FMOD_RESULT, FMOD_GEOMETRY*, int, float, float, FMOD_BOOL);
    errCheck(FMOD_Geometry_SetPolygonAttributes(geometry, index, directocclusion, reverbocclusion, doublesided));
    return 0;
}
#endif


/* FMOD_Geometry_GetPolygonAttributes(input FMOD_GEOMETRY* geometry, input int index, output float* directocclusion, output float* reverbocclusion, output FMOD_BOOL* doublesided, ) */
#ifndef FMODExt_func_FMOD_Geometry_GetPolygonAttributes
#define FMODExt_func_FMOD_Geometry_GetPolygonAttributes _FMODExt_func_FMOD_Geometry_GetPolygonAttributes
static int _FMODExt_func_FMOD_Geometry_GetPolygonAttributes(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    int index = FMODExt_check_int(L, 2);
    float directocclusion;
    float reverbocclusion;
    FMOD_BOOL doublesided;
    ensure(LL, FMOD_Geometry_GetPolygonAttributes, FMOD_RESULT, FMOD_GEOMETRY*, int, float*, float*, FMOD_BOOL*);
    errCheck(FMOD_Geometry_GetPolygonAttributes(geometry, index, &directocclusion, &reverbocclusion, &doublesided));
    FMODExt_push_float(L, directocclusion);
    FMODExt_push_float(L, reverbocclusion);
    FMODExt_push_FMOD_BOOL(L, doublesided);
    return 3;
}
#endif


/* FMOD_Geometry_SetActive(input FMOD_GEOMETRY* geometry, input FMOD_BOOL active, ) */
#ifndef FMODExt_func_FMOD_Geometry_SetActive
#define FMODExt_func_FMOD_Geometry_SetActive _FMODExt_func_FMOD_Geometry_SetActive
static int _FMODExt_func_FMOD_Geometry_SetActive(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    FMOD_BOOL active = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_Geometry_SetActive, FMOD_RESULT, FMOD_GEOMETRY*, FMOD_BOOL);
    errCheck(FMOD_Geometry_SetActive(geometry, active));
    return 0;
}
#endif


/* FMOD_Geometry_GetActive(input FMOD_GEOMETRY* geometry, output FMOD_BOOL* active, ) */
#ifndef FMODExt_func_FMOD_Geometry_GetActive
#define FMODExt_func_FMOD_Geometry_GetActive _FMODExt_func_FMOD_Geometry_GetActive
static int _FMODExt_func_FMOD_Geometry_GetActive(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    FMOD_BOOL active;
    ensure(LL, FMOD_Geometry_GetActive, FMOD_RESULT, FMOD_GEOMETRY*, FMOD_BOOL*);
    errCheck(FMOD_Geometry_GetActive(geometry, &active));
    FMODExt_push_FMOD_BOOL(L, active);
    return 1;
}
#endif


/* FMOD_Geometry_SetRotation(input FMOD_GEOMETRY* geometry, input_ptr const FMOD_VECTOR* forward, input_ptr const FMOD_VECTOR* up, ) */
#ifndef FMODExt_func_FMOD_Geometry_SetRotation
#define FMODExt_func_FMOD_Geometry_SetRotation _FMODExt_func_FMOD_Geometry_SetRotation
static int _FMODExt_func_FMOD_Geometry_SetRotation(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    const FMOD_VECTOR forward = FMODExt_check_FMOD_VECTOR(L, 2);
    const FMOD_VECTOR up = FMODExt_check_FMOD_VECTOR(L, 3);
    ensure(LL, FMOD_Geometry_SetRotation, FMOD_RESULT, FMOD_GEOMETRY*, const FMOD_VECTOR*, const FMOD_VECTOR*);
    errCheck(FMOD_Geometry_SetRotation(geometry, &forward, &up));
    return 0;
}
#endif


/* FMOD_Geometry_GetRotation(input FMOD_GEOMETRY* geometry, output FMOD_VECTOR* forward, output FMOD_VECTOR* up, ) */
#ifndef FMODExt_func_FMOD_Geometry_GetRotation
#define FMODExt_func_FMOD_Geometry_GetRotation _FMODExt_func_FMOD_Geometry_GetRotation
static int _FMODExt_func_FMOD_Geometry_GetRotation(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    FMOD_VECTOR forward;
    FMOD_VECTOR up;
    ensure(LL, FMOD_Geometry_GetRotation, FMOD_RESULT, FMOD_GEOMETRY*, FMOD_VECTOR*, FMOD_VECTOR*);
    errCheck(FMOD_Geometry_GetRotation(geometry, &forward, &up));
    FMODExt_push_FMOD_VECTOR(L, forward);
    FMODExt_push_FMOD_VECTOR(L, up);
    return 2;
}
#endif


/* FMOD_Geometry_SetPosition(input FMOD_GEOMETRY* geometry, input_ptr const FMOD_VECTOR* position, ) */
#ifndef FMODExt_func_FMOD_Geometry_SetPosition
#define FMODExt_func_FMOD_Geometry_SetPosition _FMODExt_func_FMOD_Geometry_SetPosition
static int _FMODExt_func_FMOD_Geometry_SetPosition(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    const FMOD_VECTOR position = FMODExt_check_FMOD_VECTOR(L, 2);
    ensure(LL, FMOD_Geometry_SetPosition, FMOD_RESULT, FMOD_GEOMETRY*, const FMOD_VECTOR*);
    errCheck(FMOD_Geometry_SetPosition(geometry, &position));
    return 0;
}
#endif


/* FMOD_Geometry_GetPosition(input FMOD_GEOMETRY* geometry, output FMOD_VECTOR* position, ) */
#ifndef FMODExt_func_FMOD_Geometry_GetPosition
#define FMODExt_func_FMOD_Geometry_GetPosition _FMODExt_func_FMOD_Geometry_GetPosition
static int _FMODExt_func_FMOD_Geometry_GetPosition(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    FMOD_VECTOR position;
    ensure(LL, FMOD_Geometry_GetPosition, FMOD_RESULT, FMOD_GEOMETRY*, FMOD_VECTOR*);
    errCheck(FMOD_Geometry_GetPosition(geometry, &position));
    FMODExt_push_FMOD_VECTOR(L, position);
    return 1;
}
#endif


/* FMOD_Geometry_SetScale(input FMOD_GEOMETRY* geometry, input_ptr const FMOD_VECTOR* scale, ) */
#ifndef FMODExt_func_FMOD_Geometry_SetScale
#define FMODExt_func_FMOD_Geometry_SetScale _FMODExt_func_FMOD_Geometry_SetScale
static int _FMODExt_func_FMOD_Geometry_SetScale(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    const FMOD_VECTOR scale = FMODExt_check_FMOD_VECTOR(L, 2);
    ensure(LL, FMOD_Geometry_SetScale, FMOD_RESULT, FMOD_GEOMETRY*, const FMOD_VECTOR*);
    errCheck(FMOD_Geometry_SetScale(geometry, &scale));
    return 0;
}
#endif


/* FMOD_Geometry_GetScale(input FMOD_GEOMETRY* geometry, output FMOD_VECTOR* scale, ) */
#ifndef FMODExt_func_FMOD_Geometry_GetScale
#define FMODExt_func_FMOD_Geometry_GetScale _FMODExt_func_FMOD_Geometry_GetScale
static int _FMODExt_func_FMOD_Geometry_GetScale(lua_State *L) {
    FMOD_GEOMETRY* geometry = FMODExt_check_ptr_FMOD_GEOMETRY(L, 1);
    FMOD_VECTOR scale;
    ensure(LL, FMOD_Geometry_GetScale, FMOD_RESULT, FMOD_GEOMETRY*, FMOD_VECTOR*);
    errCheck(FMOD_Geometry_GetScale(geometry, &scale));
    FMODExt_push_FMOD_VECTOR(L, scale);
    return 1;
}
#endif


/* FMOD_Geometry_Save(input FMOD_GEOMETRY* geometry, unknown void* data, output int* datasize, ) */


/* FMOD_Geometry_SetUserData(input FMOD_GEOMETRY* geometry, unknown void* userdata, ) */


/* FMOD_Geometry_GetUserData(input FMOD_GEOMETRY* geometry, unknown void** userdata, ) */


/* FMOD_Reverb3D_Release(input FMOD_REVERB3D* reverb3d, ) */
#ifndef FMODExt_func_FMOD_Reverb3D_Release
#define FMODExt_func_FMOD_Reverb3D_Release _FMODExt_func_FMOD_Reverb3D_Release
static int _FMODExt_func_FMOD_Reverb3D_Release(lua_State *L) {
    FMOD_REVERB3D* reverb3d = FMODExt_check_ptr_FMOD_REVERB3D(L, 1);
    ensure(LL, FMOD_Reverb3D_Release, FMOD_RESULT, FMOD_REVERB3D*);
    errCheck(FMOD_Reverb3D_Release(reverb3d));
    return 0;
}
#endif


/* FMOD_Reverb3D_Set3DAttributes(input FMOD_REVERB3D* reverb3d, input_ptr const FMOD_VECTOR* position, input float mindistance, input float maxdistance, ) */
#ifndef FMODExt_func_FMOD_Reverb3D_Set3DAttributes
#define FMODExt_func_FMOD_Reverb3D_Set3DAttributes _FMODExt_func_FMOD_Reverb3D_Set3DAttributes
static int _FMODExt_func_FMOD_Reverb3D_Set3DAttributes(lua_State *L) {
    FMOD_REVERB3D* reverb3d = FMODExt_check_ptr_FMOD_REVERB3D(L, 1);
    const FMOD_VECTOR position = FMODExt_check_FMOD_VECTOR(L, 2);
    float mindistance = FMODExt_check_float(L, 3);
    float maxdistance = FMODExt_check_float(L, 4);
    ensure(LL, FMOD_Reverb3D_Set3DAttributes, FMOD_RESULT, FMOD_REVERB3D*, const FMOD_VECTOR*, float, float);
    errCheck(FMOD_Reverb3D_Set3DAttributes(reverb3d, &position, mindistance, maxdistance));
    return 0;
}
#endif


/* FMOD_Reverb3D_Get3DAttributes(input FMOD_REVERB3D* reverb3d, output FMOD_VECTOR* position, output float* mindistance, output float* maxdistance, ) */
#ifndef FMODExt_func_FMOD_Reverb3D_Get3DAttributes
#define FMODExt_func_FMOD_Reverb3D_Get3DAttributes _FMODExt_func_FMOD_Reverb3D_Get3DAttributes
static int _FMODExt_func_FMOD_Reverb3D_Get3DAttributes(lua_State *L) {
    FMOD_REVERB3D* reverb3d = FMODExt_check_ptr_FMOD_REVERB3D(L, 1);
    FMOD_VECTOR position;
    float mindistance;
    float maxdistance;
    ensure(LL, FMOD_Reverb3D_Get3DAttributes, FMOD_RESULT, FMOD_REVERB3D*, FMOD_VECTOR*, float*, float*);
    errCheck(FMOD_Reverb3D_Get3DAttributes(reverb3d, &position, &mindistance, &maxdistance));
    FMODExt_push_FMOD_VECTOR(L, position);
    FMODExt_push_float(L, mindistance);
    FMODExt_push_float(L, maxdistance);
    return 3;
}
#endif


/* FMOD_Reverb3D_SetProperties(input FMOD_REVERB3D* reverb3d, input const FMOD_REVERB_PROPERTIES* properties, ) */
#ifndef FMODExt_func_FMOD_Reverb3D_SetProperties
#define FMODExt_func_FMOD_Reverb3D_SetProperties _FMODExt_func_FMOD_Reverb3D_SetProperties
static int _FMODExt_func_FMOD_Reverb3D_SetProperties(lua_State *L) {
    FMOD_REVERB3D* reverb3d = FMODExt_check_ptr_FMOD_REVERB3D(L, 1);
    const FMOD_REVERB_PROPERTIES* properties = FMODExt_check_ptr_FMOD_REVERB_PROPERTIES(L, 2);
    ensure(LL, FMOD_Reverb3D_SetProperties, FMOD_RESULT, FMOD_REVERB3D*, const FMOD_REVERB_PROPERTIES*);
    errCheck(FMOD_Reverb3D_SetProperties(reverb3d, properties));
    return 0;
}
#endif


/* FMOD_Reverb3D_GetProperties(input FMOD_REVERB3D* reverb3d, output_ptr FMOD_REVERB_PROPERTIES* properties, ) */
#ifndef FMODExt_func_FMOD_Reverb3D_GetProperties
#define FMODExt_func_FMOD_Reverb3D_GetProperties _FMODExt_func_FMOD_Reverb3D_GetProperties
static int _FMODExt_func_FMOD_Reverb3D_GetProperties(lua_State *L) {
    FMOD_REVERB3D* reverb3d = FMODExt_check_ptr_FMOD_REVERB3D(L, 1);
    FMOD_REVERB_PROPERTIES* properties = FMODExt_push_ptr_FMOD_REVERB_PROPERTIES(L, NULL);
    ensure(LL, FMOD_Reverb3D_GetProperties, FMOD_RESULT, FMOD_REVERB3D*, FMOD_REVERB_PROPERTIES*);
    errCheck(FMOD_Reverb3D_GetProperties(reverb3d, properties));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Reverb3D_SetActive(input FMOD_REVERB3D* reverb3d, input FMOD_BOOL active, ) */
#ifndef FMODExt_func_FMOD_Reverb3D_SetActive
#define FMODExt_func_FMOD_Reverb3D_SetActive _FMODExt_func_FMOD_Reverb3D_SetActive
static int _FMODExt_func_FMOD_Reverb3D_SetActive(lua_State *L) {
    FMOD_REVERB3D* reverb3d = FMODExt_check_ptr_FMOD_REVERB3D(L, 1);
    FMOD_BOOL active = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(LL, FMOD_Reverb3D_SetActive, FMOD_RESULT, FMOD_REVERB3D*, FMOD_BOOL);
    errCheck(FMOD_Reverb3D_SetActive(reverb3d, active));
    return 0;
}
#endif


/* FMOD_Reverb3D_GetActive(input FMOD_REVERB3D* reverb3d, output FMOD_BOOL* active, ) */
#ifndef FMODExt_func_FMOD_Reverb3D_GetActive
#define FMODExt_func_FMOD_Reverb3D_GetActive _FMODExt_func_FMOD_Reverb3D_GetActive
static int _FMODExt_func_FMOD_Reverb3D_GetActive(lua_State *L) {
    FMOD_REVERB3D* reverb3d = FMODExt_check_ptr_FMOD_REVERB3D(L, 1);
    FMOD_BOOL active;
    ensure(LL, FMOD_Reverb3D_GetActive, FMOD_RESULT, FMOD_REVERB3D*, FMOD_BOOL*);
    errCheck(FMOD_Reverb3D_GetActive(reverb3d, &active));
    FMODExt_push_FMOD_BOOL(L, active);
    return 1;
}
#endif


/* FMOD_Reverb3D_SetUserData(input FMOD_REVERB3D* reverb3d, unknown void* userdata, ) */


/* FMOD_Reverb3D_GetUserData(input FMOD_REVERB3D* reverb3d, unknown void** userdata, ) */


/* FMOD_Studio_ParseID(input const char* idstring, output_ptr FMOD_GUID* id, ) */
#ifndef FMODExt_func_FMOD_Studio_ParseID
#define FMODExt_func_FMOD_Studio_ParseID _FMODExt_func_FMOD_Studio_ParseID
static int _FMODExt_func_FMOD_Studio_ParseID(lua_State *L) {
    const char* idstring = FMODExt_check_ptr_char(L, 1);
    FMOD_GUID* id = FMODExt_push_ptr_FMOD_GUID(L, NULL);
    ensure(ST, FMOD_Studio_ParseID, FMOD_RESULT, const char*, FMOD_GUID*);
    errCheck(FMOD_Studio_ParseID(idstring, id));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_System_Create(output FMOD_STUDIO_SYSTEM** system, input unsigned int headerversion, ) */
#ifndef FMODExt_func_FMOD_Studio_System_Create
#define FMODExt_func_FMOD_Studio_System_Create _FMODExt_func_FMOD_Studio_System_Create
static int _FMODExt_func_FMOD_Studio_System_Create(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system;
    unsigned int headerversion = FMODExt_check_unsigned_int(L, 1);
    ensure(ST, FMOD_Studio_System_Create, FMOD_RESULT, FMOD_STUDIO_SYSTEM**, unsigned int);
    errCheck(FMOD_Studio_System_Create(&system, headerversion));
    FMODExt_push_ptr_FMOD_STUDIO_SYSTEM(L, system);
    return 1;
}
#endif


/* FMOD_Studio_System_IsValid(input FMOD_STUDIO_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_Studio_System_IsValid
#define FMODExt_func_FMOD_Studio_System_IsValid _FMODExt_func_FMOD_Studio_System_IsValid
static int _FMODExt_func_FMOD_Studio_System_IsValid(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    ensure(ST, FMOD_Studio_System_IsValid, FMOD_BOOL, FMOD_STUDIO_SYSTEM*);
    FMOD_BOOL retval = FMOD_Studio_System_IsValid(system);
    lua_pushboolean(L, retval);
    return 1;
}
#endif


/* FMOD_Studio_System_SetAdvancedSettings(input FMOD_STUDIO_SYSTEM* system, output_ptr FMOD_STUDIO_ADVANCEDSETTINGS* settings, ) */
#ifndef FMODExt_func_FMOD_Studio_System_SetAdvancedSettings
#define FMODExt_func_FMOD_Studio_System_SetAdvancedSettings _FMODExt_func_FMOD_Studio_System_SetAdvancedSettings
static int _FMODExt_func_FMOD_Studio_System_SetAdvancedSettings(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_ADVANCEDSETTINGS* settings = FMODExt_push_ptr_FMOD_STUDIO_ADVANCEDSETTINGS(L, NULL);
    ensure(ST, FMOD_Studio_System_SetAdvancedSettings, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_ADVANCEDSETTINGS*);
    errCheck(FMOD_Studio_System_SetAdvancedSettings(system, settings));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_System_GetAdvancedSettings(input FMOD_STUDIO_SYSTEM* system, output_ptr FMOD_STUDIO_ADVANCEDSETTINGS* settings, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetAdvancedSettings
#define FMODExt_func_FMOD_Studio_System_GetAdvancedSettings _FMODExt_func_FMOD_Studio_System_GetAdvancedSettings
static int _FMODExt_func_FMOD_Studio_System_GetAdvancedSettings(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_ADVANCEDSETTINGS* settings = FMODExt_push_ptr_FMOD_STUDIO_ADVANCEDSETTINGS(L, NULL);
    ensure(ST, FMOD_Studio_System_GetAdvancedSettings, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_ADVANCEDSETTINGS*);
    errCheck(FMOD_Studio_System_GetAdvancedSettings(system, settings));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_System_Initialize(input FMOD_STUDIO_SYSTEM* system, input int maxchannels, input FMOD_STUDIO_INITFLAGS studioflags, input FMOD_INITFLAGS flags, unknown void* extradriverdata, ) */


/* FMOD_Studio_System_Release(input FMOD_STUDIO_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_Studio_System_Release
#define FMODExt_func_FMOD_Studio_System_Release _FMODExt_func_FMOD_Studio_System_Release
static int _FMODExt_func_FMOD_Studio_System_Release(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    ensure(ST, FMOD_Studio_System_Release, FMOD_RESULT, FMOD_STUDIO_SYSTEM*);
    errCheck(FMOD_Studio_System_Release(system));
    return 0;
}
#endif


/* FMOD_Studio_System_Update(input FMOD_STUDIO_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_Studio_System_Update
#define FMODExt_func_FMOD_Studio_System_Update _FMODExt_func_FMOD_Studio_System_Update
static int _FMODExt_func_FMOD_Studio_System_Update(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    ensure(ST, FMOD_Studio_System_Update, FMOD_RESULT, FMOD_STUDIO_SYSTEM*);
    errCheck(FMOD_Studio_System_Update(system));
    return 0;
}
#endif


/* FMOD_Studio_System_GetCoreSystem(input FMOD_STUDIO_SYSTEM* system, output FMOD_SYSTEM** coresystem, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetCoreSystem
#define FMODExt_func_FMOD_Studio_System_GetCoreSystem _FMODExt_func_FMOD_Studio_System_GetCoreSystem
static int _FMODExt_func_FMOD_Studio_System_GetCoreSystem(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_SYSTEM* coresystem;
    ensure(ST, FMOD_Studio_System_GetCoreSystem, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_SYSTEM**);
    errCheck(FMOD_Studio_System_GetCoreSystem(system, &coresystem));
    FMODExt_push_ptr_FMOD_SYSTEM(L, coresystem);
    return 1;
}
#endif


/* FMOD_Studio_System_GetEvent(input FMOD_STUDIO_SYSTEM* system, input const char* pathOrID, output FMOD_STUDIO_EVENTDESCRIPTION** event, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetEvent
#define FMODExt_func_FMOD_Studio_System_GetEvent _FMODExt_func_FMOD_Studio_System_GetEvent
static int _FMODExt_func_FMOD_Studio_System_GetEvent(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* pathOrID = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_EVENTDESCRIPTION* event;
    ensure(ST, FMOD_Studio_System_GetEvent, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_STUDIO_EVENTDESCRIPTION**);
    errCheck(FMOD_Studio_System_GetEvent(system, pathOrID, &event));
    FMODExt_push_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, event);
    return 1;
}
#endif


/* FMOD_Studio_System_GetBus(input FMOD_STUDIO_SYSTEM* system, input const char* pathOrID, output FMOD_STUDIO_BUS** bus, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetBus
#define FMODExt_func_FMOD_Studio_System_GetBus _FMODExt_func_FMOD_Studio_System_GetBus
static int _FMODExt_func_FMOD_Studio_System_GetBus(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* pathOrID = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_BUS* bus;
    ensure(ST, FMOD_Studio_System_GetBus, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_STUDIO_BUS**);
    errCheck(FMOD_Studio_System_GetBus(system, pathOrID, &bus));
    FMODExt_push_ptr_FMOD_STUDIO_BUS(L, bus);
    return 1;
}
#endif


/* FMOD_Studio_System_GetVCA(input FMOD_STUDIO_SYSTEM* system, input const char* pathOrID, output FMOD_STUDIO_VCA** vca, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetVCA
#define FMODExt_func_FMOD_Studio_System_GetVCA _FMODExt_func_FMOD_Studio_System_GetVCA
static int _FMODExt_func_FMOD_Studio_System_GetVCA(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* pathOrID = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_VCA* vca;
    ensure(ST, FMOD_Studio_System_GetVCA, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_STUDIO_VCA**);
    errCheck(FMOD_Studio_System_GetVCA(system, pathOrID, &vca));
    FMODExt_push_ptr_FMOD_STUDIO_VCA(L, vca);
    return 1;
}
#endif


/* FMOD_Studio_System_GetBank(input FMOD_STUDIO_SYSTEM* system, input const char* pathOrID, output FMOD_STUDIO_BANK** bank, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetBank
#define FMODExt_func_FMOD_Studio_System_GetBank _FMODExt_func_FMOD_Studio_System_GetBank
static int _FMODExt_func_FMOD_Studio_System_GetBank(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* pathOrID = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_BANK* bank;
    ensure(ST, FMOD_Studio_System_GetBank, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_STUDIO_BANK**);
    errCheck(FMOD_Studio_System_GetBank(system, pathOrID, &bank));
    FMODExt_push_ptr_FMOD_STUDIO_BANK(L, bank);
    return 1;
}
#endif


/* FMOD_Studio_System_GetEventByID(input FMOD_STUDIO_SYSTEM* system, input const FMOD_GUID* id, output FMOD_STUDIO_EVENTDESCRIPTION** event, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetEventByID
#define FMODExt_func_FMOD_Studio_System_GetEventByID _FMODExt_func_FMOD_Studio_System_GetEventByID
static int _FMODExt_func_FMOD_Studio_System_GetEventByID(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const FMOD_GUID* id = FMODExt_check_ptr_FMOD_GUID(L, 2);
    FMOD_STUDIO_EVENTDESCRIPTION* event;
    ensure(ST, FMOD_Studio_System_GetEventByID, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const FMOD_GUID*, FMOD_STUDIO_EVENTDESCRIPTION**);
    errCheck(FMOD_Studio_System_GetEventByID(system, id, &event));
    FMODExt_push_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, event);
    return 1;
}
#endif


/* FMOD_Studio_System_GetBusByID(input FMOD_STUDIO_SYSTEM* system, input const FMOD_GUID* id, output FMOD_STUDIO_BUS** bus, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetBusByID
#define FMODExt_func_FMOD_Studio_System_GetBusByID _FMODExt_func_FMOD_Studio_System_GetBusByID
static int _FMODExt_func_FMOD_Studio_System_GetBusByID(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const FMOD_GUID* id = FMODExt_check_ptr_FMOD_GUID(L, 2);
    FMOD_STUDIO_BUS* bus;
    ensure(ST, FMOD_Studio_System_GetBusByID, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const FMOD_GUID*, FMOD_STUDIO_BUS**);
    errCheck(FMOD_Studio_System_GetBusByID(system, id, &bus));
    FMODExt_push_ptr_FMOD_STUDIO_BUS(L, bus);
    return 1;
}
#endif


/* FMOD_Studio_System_GetVCAByID(input FMOD_STUDIO_SYSTEM* system, input const FMOD_GUID* id, output FMOD_STUDIO_VCA** vca, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetVCAByID
#define FMODExt_func_FMOD_Studio_System_GetVCAByID _FMODExt_func_FMOD_Studio_System_GetVCAByID
static int _FMODExt_func_FMOD_Studio_System_GetVCAByID(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const FMOD_GUID* id = FMODExt_check_ptr_FMOD_GUID(L, 2);
    FMOD_STUDIO_VCA* vca;
    ensure(ST, FMOD_Studio_System_GetVCAByID, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const FMOD_GUID*, FMOD_STUDIO_VCA**);
    errCheck(FMOD_Studio_System_GetVCAByID(system, id, &vca));
    FMODExt_push_ptr_FMOD_STUDIO_VCA(L, vca);
    return 1;
}
#endif


/* FMOD_Studio_System_GetBankByID(input FMOD_STUDIO_SYSTEM* system, input const FMOD_GUID* id, output FMOD_STUDIO_BANK** bank, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetBankByID
#define FMODExt_func_FMOD_Studio_System_GetBankByID _FMODExt_func_FMOD_Studio_System_GetBankByID
static int _FMODExt_func_FMOD_Studio_System_GetBankByID(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const FMOD_GUID* id = FMODExt_check_ptr_FMOD_GUID(L, 2);
    FMOD_STUDIO_BANK* bank;
    ensure(ST, FMOD_Studio_System_GetBankByID, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const FMOD_GUID*, FMOD_STUDIO_BANK**);
    errCheck(FMOD_Studio_System_GetBankByID(system, id, &bank));
    FMODExt_push_ptr_FMOD_STUDIO_BANK(L, bank);
    return 1;
}
#endif


/* FMOD_Studio_System_GetSoundInfo(input FMOD_STUDIO_SYSTEM* system, input const char* key, output_ptr FMOD_STUDIO_SOUND_INFO* info, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetSoundInfo
#define FMODExt_func_FMOD_Studio_System_GetSoundInfo _FMODExt_func_FMOD_Studio_System_GetSoundInfo
static int _FMODExt_func_FMOD_Studio_System_GetSoundInfo(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* key = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_SOUND_INFO* info = FMODExt_push_ptr_FMOD_STUDIO_SOUND_INFO(L, NULL);
    ensure(ST, FMOD_Studio_System_GetSoundInfo, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_STUDIO_SOUND_INFO*);
    errCheck(FMOD_Studio_System_GetSoundInfo(system, key, info));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_System_GetParameterDescriptionByName(input FMOD_STUDIO_SYSTEM* system, input const char* name, output_ptr FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByName
#define FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByName _FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByName
static int _FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByName(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter = FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION(L, NULL);
    ensure(ST, FMOD_Studio_System_GetParameterDescriptionByName, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_STUDIO_PARAMETER_DESCRIPTION*);
    errCheck(FMOD_Studio_System_GetParameterDescriptionByName(system, name, parameter));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_System_GetParameterDescriptionByID(input FMOD_STUDIO_SYSTEM* system, input_deref FMOD_STUDIO_PARAMETER_ID id, output_ptr FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByID
#define FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByID _FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByID
static int _FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByID(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_PARAMETER_ID* id = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter = FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION(L, NULL);
    ensure(ST, FMOD_Studio_System_GetParameterDescriptionByID, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_PARAMETER_ID, FMOD_STUDIO_PARAMETER_DESCRIPTION*);
    errCheck(FMOD_Studio_System_GetParameterDescriptionByID(system, *id, parameter));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_System_GetParameterLabelByName(input FMOD_STUDIO_SYSTEM* system, input const char* name, input int labelindex, unknown char* label, input int size, output int* retrieved, ) */


/* FMOD_Studio_System_GetParameterLabelByID(input FMOD_STUDIO_SYSTEM* system, input_deref FMOD_STUDIO_PARAMETER_ID id, input int labelindex, unknown char* label, input int size, output int* retrieved, ) */


/* FMOD_Studio_System_GetParameterByID(input FMOD_STUDIO_SYSTEM* system, input_deref FMOD_STUDIO_PARAMETER_ID id, output float* value, output float* finalvalue, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetParameterByID
#define FMODExt_func_FMOD_Studio_System_GetParameterByID _FMODExt_func_FMOD_Studio_System_GetParameterByID
static int _FMODExt_func_FMOD_Studio_System_GetParameterByID(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_PARAMETER_ID* id = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    float value;
    float finalvalue;
    ensure(ST, FMOD_Studio_System_GetParameterByID, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_PARAMETER_ID, float*, float*);
    errCheck(FMOD_Studio_System_GetParameterByID(system, *id, &value, &finalvalue));
    FMODExt_push_float(L, value);
    FMODExt_push_float(L, finalvalue);
    return 2;
}
#endif


/* FMOD_Studio_System_SetParameterByID(input FMOD_STUDIO_SYSTEM* system, input_deref FMOD_STUDIO_PARAMETER_ID id, input float value, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_System_SetParameterByID
#define FMODExt_func_FMOD_Studio_System_SetParameterByID _FMODExt_func_FMOD_Studio_System_SetParameterByID
static int _FMODExt_func_FMOD_Studio_System_SetParameterByID(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_PARAMETER_ID* id = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    float value = FMODExt_check_float(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_System_SetParameterByID, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_PARAMETER_ID, float, FMOD_BOOL);
    errCheck(FMOD_Studio_System_SetParameterByID(system, *id, value, ignoreseekspeed));
    return 0;
}
#endif


/* FMOD_Studio_System_SetParameterByIDWithLabel(input FMOD_STUDIO_SYSTEM* system, input_deref FMOD_STUDIO_PARAMETER_ID id, input const char* label, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_System_SetParameterByIDWithLabel
#define FMODExt_func_FMOD_Studio_System_SetParameterByIDWithLabel _FMODExt_func_FMOD_Studio_System_SetParameterByIDWithLabel
static int _FMODExt_func_FMOD_Studio_System_SetParameterByIDWithLabel(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_PARAMETER_ID* id = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    const char* label = FMODExt_check_ptr_char(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_System_SetParameterByIDWithLabel, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_PARAMETER_ID, const char*, FMOD_BOOL);
    errCheck(FMOD_Studio_System_SetParameterByIDWithLabel(system, *id, label, ignoreseekspeed));
    return 0;
}
#endif


/* FMOD_Studio_System_SetParametersByIDs(input FMOD_STUDIO_SYSTEM* system, input const FMOD_STUDIO_PARAMETER_ID* ids, output float* values, input int count, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_System_SetParametersByIDs
#define FMODExt_func_FMOD_Studio_System_SetParametersByIDs _FMODExt_func_FMOD_Studio_System_SetParametersByIDs
static int _FMODExt_func_FMOD_Studio_System_SetParametersByIDs(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const FMOD_STUDIO_PARAMETER_ID* ids = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    float values;
    int count = FMODExt_check_int(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_System_SetParametersByIDs, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const FMOD_STUDIO_PARAMETER_ID*, float*, int, FMOD_BOOL);
    errCheck(FMOD_Studio_System_SetParametersByIDs(system, ids, &values, count, ignoreseekspeed));
    FMODExt_push_float(L, values);
    return 1;
}
#endif


/* FMOD_Studio_System_GetParameterByName(input FMOD_STUDIO_SYSTEM* system, input const char* name, output float* value, output float* finalvalue, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetParameterByName
#define FMODExt_func_FMOD_Studio_System_GetParameterByName _FMODExt_func_FMOD_Studio_System_GetParameterByName
static int _FMODExt_func_FMOD_Studio_System_GetParameterByName(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    float value;
    float finalvalue;
    ensure(ST, FMOD_Studio_System_GetParameterByName, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, float*, float*);
    errCheck(FMOD_Studio_System_GetParameterByName(system, name, &value, &finalvalue));
    FMODExt_push_float(L, value);
    FMODExt_push_float(L, finalvalue);
    return 2;
}
#endif


/* FMOD_Studio_System_SetParameterByName(input FMOD_STUDIO_SYSTEM* system, input const char* name, input float value, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_System_SetParameterByName
#define FMODExt_func_FMOD_Studio_System_SetParameterByName _FMODExt_func_FMOD_Studio_System_SetParameterByName
static int _FMODExt_func_FMOD_Studio_System_SetParameterByName(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    float value = FMODExt_check_float(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_System_SetParameterByName, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, float, FMOD_BOOL);
    errCheck(FMOD_Studio_System_SetParameterByName(system, name, value, ignoreseekspeed));
    return 0;
}
#endif


/* FMOD_Studio_System_SetParameterByNameWithLabel(input FMOD_STUDIO_SYSTEM* system, input const char* name, input const char* label, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_System_SetParameterByNameWithLabel
#define FMODExt_func_FMOD_Studio_System_SetParameterByNameWithLabel _FMODExt_func_FMOD_Studio_System_SetParameterByNameWithLabel
static int _FMODExt_func_FMOD_Studio_System_SetParameterByNameWithLabel(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    const char* label = FMODExt_check_ptr_char(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_System_SetParameterByNameWithLabel, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, const char*, FMOD_BOOL);
    errCheck(FMOD_Studio_System_SetParameterByNameWithLabel(system, name, label, ignoreseekspeed));
    return 0;
}
#endif


/* FMOD_Studio_System_LookupID(input FMOD_STUDIO_SYSTEM* system, input const char* path, output_ptr FMOD_GUID* id, ) */
#ifndef FMODExt_func_FMOD_Studio_System_LookupID
#define FMODExt_func_FMOD_Studio_System_LookupID _FMODExt_func_FMOD_Studio_System_LookupID
static int _FMODExt_func_FMOD_Studio_System_LookupID(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* path = FMODExt_check_ptr_char(L, 2);
    FMOD_GUID* id = FMODExt_push_ptr_FMOD_GUID(L, NULL);
    ensure(ST, FMOD_Studio_System_LookupID, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_GUID*);
    errCheck(FMOD_Studio_System_LookupID(system, path, id));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_System_LookupPath(input FMOD_STUDIO_SYSTEM* system, input const FMOD_GUID* id, unknown char* path, input int size, output int* retrieved, ) */


/* FMOD_Studio_System_GetNumListeners(input FMOD_STUDIO_SYSTEM* system, output int* numlisteners, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetNumListeners
#define FMODExt_func_FMOD_Studio_System_GetNumListeners _FMODExt_func_FMOD_Studio_System_GetNumListeners
static int _FMODExt_func_FMOD_Studio_System_GetNumListeners(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    int numlisteners;
    ensure(ST, FMOD_Studio_System_GetNumListeners, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, int*);
    errCheck(FMOD_Studio_System_GetNumListeners(system, &numlisteners));
    FMODExt_push_int(L, numlisteners);
    return 1;
}
#endif


/* FMOD_Studio_System_SetNumListeners(input FMOD_STUDIO_SYSTEM* system, input int numlisteners, ) */
#ifndef FMODExt_func_FMOD_Studio_System_SetNumListeners
#define FMODExt_func_FMOD_Studio_System_SetNumListeners _FMODExt_func_FMOD_Studio_System_SetNumListeners
static int _FMODExt_func_FMOD_Studio_System_SetNumListeners(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    int numlisteners = FMODExt_check_int(L, 2);
    ensure(ST, FMOD_Studio_System_SetNumListeners, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, int);
    errCheck(FMOD_Studio_System_SetNumListeners(system, numlisteners));
    return 0;
}
#endif


/* FMOD_Studio_System_GetListenerAttributes(input FMOD_STUDIO_SYSTEM* system, input int index, output_ptr FMOD_3D_ATTRIBUTES* attributes, output FMOD_VECTOR* attenuationposition, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetListenerAttributes
#define FMODExt_func_FMOD_Studio_System_GetListenerAttributes _FMODExt_func_FMOD_Studio_System_GetListenerAttributes
static int _FMODExt_func_FMOD_Studio_System_GetListenerAttributes(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_3D_ATTRIBUTES* attributes = FMODExt_push_ptr_FMOD_3D_ATTRIBUTES(L, NULL);
    FMOD_VECTOR attenuationposition;
    ensure(ST, FMOD_Studio_System_GetListenerAttributes, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, int, FMOD_3D_ATTRIBUTES*, FMOD_VECTOR*);
    errCheck(FMOD_Studio_System_GetListenerAttributes(system, index, attributes, &attenuationposition));
    lua_pushvalue(L, -1);
    FMODExt_push_FMOD_VECTOR(L, attenuationposition);
    return 2;
}
#endif


/* FMOD_Studio_System_SetListenerAttributes(input FMOD_STUDIO_SYSTEM* system, input int index, input const FMOD_3D_ATTRIBUTES* attributes, input_ptr const FMOD_VECTOR* attenuationposition, ) */
#ifndef FMODExt_func_FMOD_Studio_System_SetListenerAttributes
#define FMODExt_func_FMOD_Studio_System_SetListenerAttributes _FMODExt_func_FMOD_Studio_System_SetListenerAttributes
static int _FMODExt_func_FMOD_Studio_System_SetListenerAttributes(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    int index = FMODExt_check_int(L, 2);
    const FMOD_3D_ATTRIBUTES* attributes = FMODExt_check_ptr_FMOD_3D_ATTRIBUTES(L, 3);
    const FMOD_VECTOR attenuationposition = FMODExt_check_FMOD_VECTOR(L, 4);
    ensure(ST, FMOD_Studio_System_SetListenerAttributes, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, int, const FMOD_3D_ATTRIBUTES*, const FMOD_VECTOR*);
    errCheck(FMOD_Studio_System_SetListenerAttributes(system, index, attributes, &attenuationposition));
    return 0;
}
#endif


/* FMOD_Studio_System_GetListenerWeight(input FMOD_STUDIO_SYSTEM* system, input int index, output float* weight, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetListenerWeight
#define FMODExt_func_FMOD_Studio_System_GetListenerWeight _FMODExt_func_FMOD_Studio_System_GetListenerWeight
static int _FMODExt_func_FMOD_Studio_System_GetListenerWeight(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    int index = FMODExt_check_int(L, 2);
    float weight;
    ensure(ST, FMOD_Studio_System_GetListenerWeight, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, int, float*);
    errCheck(FMOD_Studio_System_GetListenerWeight(system, index, &weight));
    FMODExt_push_float(L, weight);
    return 1;
}
#endif


/* FMOD_Studio_System_SetListenerWeight(input FMOD_STUDIO_SYSTEM* system, input int index, input float weight, ) */
#ifndef FMODExt_func_FMOD_Studio_System_SetListenerWeight
#define FMODExt_func_FMOD_Studio_System_SetListenerWeight _FMODExt_func_FMOD_Studio_System_SetListenerWeight
static int _FMODExt_func_FMOD_Studio_System_SetListenerWeight(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    int index = FMODExt_check_int(L, 2);
    float weight = FMODExt_check_float(L, 3);
    ensure(ST, FMOD_Studio_System_SetListenerWeight, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, int, float);
    errCheck(FMOD_Studio_System_SetListenerWeight(system, index, weight));
    return 0;
}
#endif


/* FMOD_Studio_System_LoadBankFile(input FMOD_STUDIO_SYSTEM* system, input const char* filename, input FMOD_STUDIO_LOAD_BANK_FLAGS flags, output FMOD_STUDIO_BANK** bank, ) */
#ifndef FMODExt_func_FMOD_Studio_System_LoadBankFile
#define FMODExt_func_FMOD_Studio_System_LoadBankFile _FMODExt_func_FMOD_Studio_System_LoadBankFile
static int _FMODExt_func_FMOD_Studio_System_LoadBankFile(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* filename = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMODExt_check_unsigned_int(L, 3);
    FMOD_STUDIO_BANK* bank;
    ensure(ST, FMOD_Studio_System_LoadBankFile, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_STUDIO_LOAD_BANK_FLAGS, FMOD_STUDIO_BANK**);
    errCheck(FMOD_Studio_System_LoadBankFile(system, filename, flags, &bank));
    FMODExt_push_ptr_FMOD_STUDIO_BANK(L, bank);
    return 1;
}
#endif


/* FMOD_Studio_System_LoadBankMemory(input FMOD_STUDIO_SYSTEM* system, input const char* buffer, input int length, input FMOD_STUDIO_LOAD_MEMORY_MODE mode, input FMOD_STUDIO_LOAD_BANK_FLAGS flags, output FMOD_STUDIO_BANK** bank, ) */
#ifndef FMODExt_func_FMOD_Studio_System_LoadBankMemory
#define FMODExt_func_FMOD_Studio_System_LoadBankMemory _FMODExt_func_FMOD_Studio_System_LoadBankMemory
static int _FMODExt_func_FMOD_Studio_System_LoadBankMemory(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* buffer = FMODExt_check_ptr_char(L, 2);
    int length = FMODExt_check_int(L, 3);
    FMOD_STUDIO_LOAD_MEMORY_MODE mode = FMODExt_check_FMOD_STUDIO_LOAD_MEMORY_MODE(L, 4);
    FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMODExt_check_unsigned_int(L, 5);
    FMOD_STUDIO_BANK* bank;
    ensure(ST, FMOD_Studio_System_LoadBankMemory, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, int, FMOD_STUDIO_LOAD_MEMORY_MODE, FMOD_STUDIO_LOAD_BANK_FLAGS, FMOD_STUDIO_BANK**);
    errCheck(FMOD_Studio_System_LoadBankMemory(system, buffer, length, mode, flags, &bank));
    FMODExt_push_ptr_FMOD_STUDIO_BANK(L, bank);
    return 1;
}
#endif


/* FMOD_Studio_System_LoadBankCustom(input FMOD_STUDIO_SYSTEM* system, input const FMOD_STUDIO_BANK_INFO* info, input FMOD_STUDIO_LOAD_BANK_FLAGS flags, output FMOD_STUDIO_BANK** bank, ) */
#ifndef FMODExt_func_FMOD_Studio_System_LoadBankCustom
#define FMODExt_func_FMOD_Studio_System_LoadBankCustom _FMODExt_func_FMOD_Studio_System_LoadBankCustom
static int _FMODExt_func_FMOD_Studio_System_LoadBankCustom(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const FMOD_STUDIO_BANK_INFO* info = FMODExt_check_ptr_FMOD_STUDIO_BANK_INFO(L, 2);
    FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMODExt_check_unsigned_int(L, 3);
    FMOD_STUDIO_BANK* bank;
    ensure(ST, FMOD_Studio_System_LoadBankCustom, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const FMOD_STUDIO_BANK_INFO*, FMOD_STUDIO_LOAD_BANK_FLAGS, FMOD_STUDIO_BANK**);
    errCheck(FMOD_Studio_System_LoadBankCustom(system, info, flags, &bank));
    FMODExt_push_ptr_FMOD_STUDIO_BANK(L, bank);
    return 1;
}
#endif


/* FMOD_Studio_System_RegisterPlugin(input FMOD_STUDIO_SYSTEM* system, input const FMOD_DSP_DESCRIPTION* description, ) */
#ifndef FMODExt_func_FMOD_Studio_System_RegisterPlugin
#define FMODExt_func_FMOD_Studio_System_RegisterPlugin _FMODExt_func_FMOD_Studio_System_RegisterPlugin
static int _FMODExt_func_FMOD_Studio_System_RegisterPlugin(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const FMOD_DSP_DESCRIPTION* description = FMODExt_check_ptr_FMOD_DSP_DESCRIPTION(L, 2);
    ensure(ST, FMOD_Studio_System_RegisterPlugin, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const FMOD_DSP_DESCRIPTION*);
    errCheck(FMOD_Studio_System_RegisterPlugin(system, description));
    return 0;
}
#endif


/* FMOD_Studio_System_UnregisterPlugin(input FMOD_STUDIO_SYSTEM* system, input const char* name, ) */
#ifndef FMODExt_func_FMOD_Studio_System_UnregisterPlugin
#define FMODExt_func_FMOD_Studio_System_UnregisterPlugin _FMODExt_func_FMOD_Studio_System_UnregisterPlugin
static int _FMODExt_func_FMOD_Studio_System_UnregisterPlugin(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    ensure(ST, FMOD_Studio_System_UnregisterPlugin, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*);
    errCheck(FMOD_Studio_System_UnregisterPlugin(system, name));
    return 0;
}
#endif


/* FMOD_Studio_System_UnloadAll(input FMOD_STUDIO_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_Studio_System_UnloadAll
#define FMODExt_func_FMOD_Studio_System_UnloadAll _FMODExt_func_FMOD_Studio_System_UnloadAll
static int _FMODExt_func_FMOD_Studio_System_UnloadAll(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    ensure(ST, FMOD_Studio_System_UnloadAll, FMOD_RESULT, FMOD_STUDIO_SYSTEM*);
    errCheck(FMOD_Studio_System_UnloadAll(system));
    return 0;
}
#endif


/* FMOD_Studio_System_FlushCommands(input FMOD_STUDIO_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_Studio_System_FlushCommands
#define FMODExt_func_FMOD_Studio_System_FlushCommands _FMODExt_func_FMOD_Studio_System_FlushCommands
static int _FMODExt_func_FMOD_Studio_System_FlushCommands(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    ensure(ST, FMOD_Studio_System_FlushCommands, FMOD_RESULT, FMOD_STUDIO_SYSTEM*);
    errCheck(FMOD_Studio_System_FlushCommands(system));
    return 0;
}
#endif


/* FMOD_Studio_System_FlushSampleLoading(input FMOD_STUDIO_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_Studio_System_FlushSampleLoading
#define FMODExt_func_FMOD_Studio_System_FlushSampleLoading _FMODExt_func_FMOD_Studio_System_FlushSampleLoading
static int _FMODExt_func_FMOD_Studio_System_FlushSampleLoading(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    ensure(ST, FMOD_Studio_System_FlushSampleLoading, FMOD_RESULT, FMOD_STUDIO_SYSTEM*);
    errCheck(FMOD_Studio_System_FlushSampleLoading(system));
    return 0;
}
#endif


/* FMOD_Studio_System_StartCommandCapture(input FMOD_STUDIO_SYSTEM* system, input const char* filename, input FMOD_STUDIO_COMMANDCAPTURE_FLAGS flags, ) */
#ifndef FMODExt_func_FMOD_Studio_System_StartCommandCapture
#define FMODExt_func_FMOD_Studio_System_StartCommandCapture _FMODExt_func_FMOD_Studio_System_StartCommandCapture
static int _FMODExt_func_FMOD_Studio_System_StartCommandCapture(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* filename = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_COMMANDCAPTURE_FLAGS flags = FMODExt_check_unsigned_int(L, 3);
    ensure(ST, FMOD_Studio_System_StartCommandCapture, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_STUDIO_COMMANDCAPTURE_FLAGS);
    errCheck(FMOD_Studio_System_StartCommandCapture(system, filename, flags));
    return 0;
}
#endif


/* FMOD_Studio_System_StopCommandCapture(input FMOD_STUDIO_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_Studio_System_StopCommandCapture
#define FMODExt_func_FMOD_Studio_System_StopCommandCapture _FMODExt_func_FMOD_Studio_System_StopCommandCapture
static int _FMODExt_func_FMOD_Studio_System_StopCommandCapture(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    ensure(ST, FMOD_Studio_System_StopCommandCapture, FMOD_RESULT, FMOD_STUDIO_SYSTEM*);
    errCheck(FMOD_Studio_System_StopCommandCapture(system));
    return 0;
}
#endif


/* FMOD_Studio_System_LoadCommandReplay(input FMOD_STUDIO_SYSTEM* system, input const char* filename, input FMOD_STUDIO_COMMANDREPLAY_FLAGS flags, output FMOD_STUDIO_COMMANDREPLAY** replay, ) */
#ifndef FMODExt_func_FMOD_Studio_System_LoadCommandReplay
#define FMODExt_func_FMOD_Studio_System_LoadCommandReplay _FMODExt_func_FMOD_Studio_System_LoadCommandReplay
static int _FMODExt_func_FMOD_Studio_System_LoadCommandReplay(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    const char* filename = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_COMMANDREPLAY_FLAGS flags = FMODExt_check_unsigned_int(L, 3);
    FMOD_STUDIO_COMMANDREPLAY* replay;
    ensure(ST, FMOD_Studio_System_LoadCommandReplay, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, const char*, FMOD_STUDIO_COMMANDREPLAY_FLAGS, FMOD_STUDIO_COMMANDREPLAY**);
    errCheck(FMOD_Studio_System_LoadCommandReplay(system, filename, flags, &replay));
    FMODExt_push_ptr_FMOD_STUDIO_COMMANDREPLAY(L, replay);
    return 1;
}
#endif


/* FMOD_Studio_System_GetBankCount(input FMOD_STUDIO_SYSTEM* system, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetBankCount
#define FMODExt_func_FMOD_Studio_System_GetBankCount _FMODExt_func_FMOD_Studio_System_GetBankCount
static int _FMODExt_func_FMOD_Studio_System_GetBankCount(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    int count;
    ensure(ST, FMOD_Studio_System_GetBankCount, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, int*);
    errCheck(FMOD_Studio_System_GetBankCount(system, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_System_GetBankList(input FMOD_STUDIO_SYSTEM* system, output FMOD_STUDIO_BANK** array, input int capacity, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetBankList
#define FMODExt_func_FMOD_Studio_System_GetBankList _FMODExt_func_FMOD_Studio_System_GetBankList
static int _FMODExt_func_FMOD_Studio_System_GetBankList(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_BANK* array;
    int capacity = FMODExt_check_int(L, 2);
    int count;
    ensure(ST, FMOD_Studio_System_GetBankList, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_BANK**, int, int*);
    errCheck(FMOD_Studio_System_GetBankList(system, &array, capacity, &count));
    FMODExt_push_ptr_FMOD_STUDIO_BANK(L, array);
    FMODExt_push_int(L, count);
    return 2;
}
#endif


/* FMOD_Studio_System_GetParameterDescriptionCount(input FMOD_STUDIO_SYSTEM* system, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetParameterDescriptionCount
#define FMODExt_func_FMOD_Studio_System_GetParameterDescriptionCount _FMODExt_func_FMOD_Studio_System_GetParameterDescriptionCount
static int _FMODExt_func_FMOD_Studio_System_GetParameterDescriptionCount(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    int count;
    ensure(ST, FMOD_Studio_System_GetParameterDescriptionCount, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, int*);
    errCheck(FMOD_Studio_System_GetParameterDescriptionCount(system, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_System_GetParameterDescriptionList(input FMOD_STUDIO_SYSTEM* system, output_ptr FMOD_STUDIO_PARAMETER_DESCRIPTION* array, input int capacity, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetParameterDescriptionList
#define FMODExt_func_FMOD_Studio_System_GetParameterDescriptionList _FMODExt_func_FMOD_Studio_System_GetParameterDescriptionList
static int _FMODExt_func_FMOD_Studio_System_GetParameterDescriptionList(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_PARAMETER_DESCRIPTION* array = FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION(L, NULL);
    int capacity = FMODExt_check_int(L, 2);
    int count;
    ensure(ST, FMOD_Studio_System_GetParameterDescriptionList, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_PARAMETER_DESCRIPTION*, int, int*);
    errCheck(FMOD_Studio_System_GetParameterDescriptionList(system, array, capacity, &count));
    lua_pushvalue(L, -1);
    FMODExt_push_int(L, count);
    return 2;
}
#endif


/* FMOD_Studio_System_GetCPUUsage(input FMOD_STUDIO_SYSTEM* system, output_ptr FMOD_STUDIO_CPU_USAGE* usage, output_ptr FMOD_CPU_USAGE* usage_core, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetCPUUsage
#define FMODExt_func_FMOD_Studio_System_GetCPUUsage _FMODExt_func_FMOD_Studio_System_GetCPUUsage
static int _FMODExt_func_FMOD_Studio_System_GetCPUUsage(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_CPU_USAGE* usage = FMODExt_push_ptr_FMOD_STUDIO_CPU_USAGE(L, NULL);
    FMOD_CPU_USAGE* usage_core = FMODExt_push_ptr_FMOD_CPU_USAGE(L, NULL);
    ensure(ST, FMOD_Studio_System_GetCPUUsage, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_CPU_USAGE*, FMOD_CPU_USAGE*);
    errCheck(FMOD_Studio_System_GetCPUUsage(system, usage, usage_core));
    lua_pushvalue(L, -2);
    lua_pushvalue(L, -2);
    return 2;
}
#endif


/* FMOD_Studio_System_GetBufferUsage(input FMOD_STUDIO_SYSTEM* system, output_ptr FMOD_STUDIO_BUFFER_USAGE* usage, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetBufferUsage
#define FMODExt_func_FMOD_Studio_System_GetBufferUsage _FMODExt_func_FMOD_Studio_System_GetBufferUsage
static int _FMODExt_func_FMOD_Studio_System_GetBufferUsage(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_BUFFER_USAGE* usage = FMODExt_push_ptr_FMOD_STUDIO_BUFFER_USAGE(L, NULL);
    ensure(ST, FMOD_Studio_System_GetBufferUsage, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_BUFFER_USAGE*);
    errCheck(FMOD_Studio_System_GetBufferUsage(system, usage));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_System_ResetBufferUsage(input FMOD_STUDIO_SYSTEM* system, ) */
#ifndef FMODExt_func_FMOD_Studio_System_ResetBufferUsage
#define FMODExt_func_FMOD_Studio_System_ResetBufferUsage _FMODExt_func_FMOD_Studio_System_ResetBufferUsage
static int _FMODExt_func_FMOD_Studio_System_ResetBufferUsage(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    ensure(ST, FMOD_Studio_System_ResetBufferUsage, FMOD_RESULT, FMOD_STUDIO_SYSTEM*);
    errCheck(FMOD_Studio_System_ResetBufferUsage(system));
    return 0;
}
#endif


/* FMOD_Studio_System_SetCallback(input FMOD_STUDIO_SYSTEM* system, unknown FMOD_STUDIO_SYSTEM_CALLBACK callback, input FMOD_STUDIO_SYSTEM_CALLBACK_TYPE callbackmask, ) */


/* FMOD_Studio_System_SetUserData(input FMOD_STUDIO_SYSTEM* system, unknown void* userdata, ) */


/* FMOD_Studio_System_GetUserData(input FMOD_STUDIO_SYSTEM* system, unknown void** userdata, ) */


/* FMOD_Studio_System_GetMemoryUsage(input FMOD_STUDIO_SYSTEM* system, output_ptr FMOD_STUDIO_MEMORY_USAGE* memoryusage, ) */
#ifndef FMODExt_func_FMOD_Studio_System_GetMemoryUsage
#define FMODExt_func_FMOD_Studio_System_GetMemoryUsage _FMODExt_func_FMOD_Studio_System_GetMemoryUsage
static int _FMODExt_func_FMOD_Studio_System_GetMemoryUsage(lua_State *L) {
    FMOD_STUDIO_SYSTEM* system = FMODExt_check_ptr_FMOD_STUDIO_SYSTEM(L, 1);
    FMOD_STUDIO_MEMORY_USAGE* memoryusage = FMODExt_push_ptr_FMOD_STUDIO_MEMORY_USAGE(L, NULL);
    ensure(ST, FMOD_Studio_System_GetMemoryUsage, FMOD_RESULT, FMOD_STUDIO_SYSTEM*, FMOD_STUDIO_MEMORY_USAGE*);
    errCheck(FMOD_Studio_System_GetMemoryUsage(system, memoryusage));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_IsValid(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_IsValid
#define FMODExt_func_FMOD_Studio_EventDescription_IsValid _FMODExt_func_FMOD_Studio_EventDescription_IsValid
static int _FMODExt_func_FMOD_Studio_EventDescription_IsValid(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    ensure(ST, FMOD_Studio_EventDescription_IsValid, FMOD_BOOL, FMOD_STUDIO_EVENTDESCRIPTION*);
    FMOD_BOOL retval = FMOD_Studio_EventDescription_IsValid(eventdescription);
    lua_pushboolean(L, retval);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetID(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output_ptr FMOD_GUID* id, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetID
#define FMODExt_func_FMOD_Studio_EventDescription_GetID _FMODExt_func_FMOD_Studio_EventDescription_GetID
static int _FMODExt_func_FMOD_Studio_EventDescription_GetID(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_GUID* id = FMODExt_push_ptr_FMOD_GUID(L, NULL);
    ensure(ST, FMOD_Studio_EventDescription_GetID, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_GUID*);
    errCheck(FMOD_Studio_EventDescription_GetID(eventdescription, id));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetPath(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, unknown char* path, input int size, output int* retrieved, ) */


/* FMOD_Studio_EventDescription_GetParameterDescriptionCount(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionCount
#define FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionCount _FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionCount
static int _FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionCount(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    int count;
    ensure(ST, FMOD_Studio_EventDescription_GetParameterDescriptionCount, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, int*);
    errCheck(FMOD_Studio_EventDescription_GetParameterDescriptionCount(eventdescription, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, input int index, output_ptr FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByIndex
#define FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByIndex _FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByIndex
static int _FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter = FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION(L, NULL);
    ensure(ST, FMOD_Studio_EventDescription_GetParameterDescriptionByIndex, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, int, FMOD_STUDIO_PARAMETER_DESCRIPTION*);
    errCheck(FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(eventdescription, index, parameter));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetParameterDescriptionByName(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, input const char* name, output_ptr FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByName
#define FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByName _FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByName
static int _FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByName(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter = FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION(L, NULL);
    ensure(ST, FMOD_Studio_EventDescription_GetParameterDescriptionByName, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, const char*, FMOD_STUDIO_PARAMETER_DESCRIPTION*);
    errCheck(FMOD_Studio_EventDescription_GetParameterDescriptionByName(eventdescription, name, parameter));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetParameterDescriptionByID(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, input_deref FMOD_STUDIO_PARAMETER_ID id, output_ptr FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByID
#define FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByID _FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByID
static int _FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByID(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_STUDIO_PARAMETER_ID* id = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    FMOD_STUDIO_PARAMETER_DESCRIPTION* parameter = FMODExt_push_ptr_FMOD_STUDIO_PARAMETER_DESCRIPTION(L, NULL);
    ensure(ST, FMOD_Studio_EventDescription_GetParameterDescriptionByID, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_STUDIO_PARAMETER_ID, FMOD_STUDIO_PARAMETER_DESCRIPTION*);
    errCheck(FMOD_Studio_EventDescription_GetParameterDescriptionByID(eventdescription, *id, parameter));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetParameterLabelByIndex(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, input int index, input int labelindex, unknown char* label, input int size, output int* retrieved, ) */


/* FMOD_Studio_EventDescription_GetParameterLabelByName(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, input const char* name, input int labelindex, unknown char* label, input int size, output int* retrieved, ) */


/* FMOD_Studio_EventDescription_GetParameterLabelByID(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, input_deref FMOD_STUDIO_PARAMETER_ID id, input int labelindex, unknown char* label, input int size, output int* retrieved, ) */


/* FMOD_Studio_EventDescription_GetUserPropertyCount(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyCount
#define FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyCount _FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyCount
static int _FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyCount(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    int count;
    ensure(ST, FMOD_Studio_EventDescription_GetUserPropertyCount, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, int*);
    errCheck(FMOD_Studio_EventDescription_GetUserPropertyCount(eventdescription, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetUserPropertyByIndex(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, input int index, output_ptr FMOD_STUDIO_USER_PROPERTY* property, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyByIndex
#define FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyByIndex _FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyByIndex
static int _FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyByIndex(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    int index = FMODExt_check_int(L, 2);
    FMOD_STUDIO_USER_PROPERTY* property = FMODExt_push_ptr_FMOD_STUDIO_USER_PROPERTY(L, NULL);
    ensure(ST, FMOD_Studio_EventDescription_GetUserPropertyByIndex, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, int, FMOD_STUDIO_USER_PROPERTY*);
    errCheck(FMOD_Studio_EventDescription_GetUserPropertyByIndex(eventdescription, index, property));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetUserProperty(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, input const char* name, output_ptr FMOD_STUDIO_USER_PROPERTY* property, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetUserProperty
#define FMODExt_func_FMOD_Studio_EventDescription_GetUserProperty _FMODExt_func_FMOD_Studio_EventDescription_GetUserProperty
static int _FMODExt_func_FMOD_Studio_EventDescription_GetUserProperty(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    FMOD_STUDIO_USER_PROPERTY* property = FMODExt_push_ptr_FMOD_STUDIO_USER_PROPERTY(L, NULL);
    ensure(ST, FMOD_Studio_EventDescription_GetUserProperty, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, const char*, FMOD_STUDIO_USER_PROPERTY*);
    errCheck(FMOD_Studio_EventDescription_GetUserProperty(eventdescription, name, property));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetLength(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output int* length, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetLength
#define FMODExt_func_FMOD_Studio_EventDescription_GetLength _FMODExt_func_FMOD_Studio_EventDescription_GetLength
static int _FMODExt_func_FMOD_Studio_EventDescription_GetLength(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    int length;
    ensure(ST, FMOD_Studio_EventDescription_GetLength, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, int*);
    errCheck(FMOD_Studio_EventDescription_GetLength(eventdescription, &length));
    FMODExt_push_int(L, length);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetMinMaxDistance(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output float* min, output float* max, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetMinMaxDistance
#define FMODExt_func_FMOD_Studio_EventDescription_GetMinMaxDistance _FMODExt_func_FMOD_Studio_EventDescription_GetMinMaxDistance
static int _FMODExt_func_FMOD_Studio_EventDescription_GetMinMaxDistance(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    float min;
    float max;
    ensure(ST, FMOD_Studio_EventDescription_GetMinMaxDistance, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, float*, float*);
    errCheck(FMOD_Studio_EventDescription_GetMinMaxDistance(eventdescription, &min, &max));
    FMODExt_push_float(L, min);
    FMODExt_push_float(L, max);
    return 2;
}
#endif


/* FMOD_Studio_EventDescription_GetSoundSize(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output float* size, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetSoundSize
#define FMODExt_func_FMOD_Studio_EventDescription_GetSoundSize _FMODExt_func_FMOD_Studio_EventDescription_GetSoundSize
static int _FMODExt_func_FMOD_Studio_EventDescription_GetSoundSize(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    float size;
    ensure(ST, FMOD_Studio_EventDescription_GetSoundSize, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, float*);
    errCheck(FMOD_Studio_EventDescription_GetSoundSize(eventdescription, &size));
    FMODExt_push_float(L, size);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_IsSnapshot(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output FMOD_BOOL* snapshot, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_IsSnapshot
#define FMODExt_func_FMOD_Studio_EventDescription_IsSnapshot _FMODExt_func_FMOD_Studio_EventDescription_IsSnapshot
static int _FMODExt_func_FMOD_Studio_EventDescription_IsSnapshot(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_BOOL snapshot;
    ensure(ST, FMOD_Studio_EventDescription_IsSnapshot, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_BOOL*);
    errCheck(FMOD_Studio_EventDescription_IsSnapshot(eventdescription, &snapshot));
    FMODExt_push_FMOD_BOOL(L, snapshot);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_IsOneshot(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output FMOD_BOOL* oneshot, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_IsOneshot
#define FMODExt_func_FMOD_Studio_EventDescription_IsOneshot _FMODExt_func_FMOD_Studio_EventDescription_IsOneshot
static int _FMODExt_func_FMOD_Studio_EventDescription_IsOneshot(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_BOOL oneshot;
    ensure(ST, FMOD_Studio_EventDescription_IsOneshot, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_BOOL*);
    errCheck(FMOD_Studio_EventDescription_IsOneshot(eventdescription, &oneshot));
    FMODExt_push_FMOD_BOOL(L, oneshot);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_IsStream(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output FMOD_BOOL* isStream, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_IsStream
#define FMODExt_func_FMOD_Studio_EventDescription_IsStream _FMODExt_func_FMOD_Studio_EventDescription_IsStream
static int _FMODExt_func_FMOD_Studio_EventDescription_IsStream(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_BOOL isStream;
    ensure(ST, FMOD_Studio_EventDescription_IsStream, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_BOOL*);
    errCheck(FMOD_Studio_EventDescription_IsStream(eventdescription, &isStream));
    FMODExt_push_FMOD_BOOL(L, isStream);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_Is3D(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output FMOD_BOOL* is3D, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_Is3D
#define FMODExt_func_FMOD_Studio_EventDescription_Is3D _FMODExt_func_FMOD_Studio_EventDescription_Is3D
static int _FMODExt_func_FMOD_Studio_EventDescription_Is3D(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_BOOL is3D;
    ensure(ST, FMOD_Studio_EventDescription_Is3D, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_BOOL*);
    errCheck(FMOD_Studio_EventDescription_Is3D(eventdescription, &is3D));
    FMODExt_push_FMOD_BOOL(L, is3D);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_IsDopplerEnabled(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output FMOD_BOOL* doppler, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_IsDopplerEnabled
#define FMODExt_func_FMOD_Studio_EventDescription_IsDopplerEnabled _FMODExt_func_FMOD_Studio_EventDescription_IsDopplerEnabled
static int _FMODExt_func_FMOD_Studio_EventDescription_IsDopplerEnabled(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_BOOL doppler;
    ensure(ST, FMOD_Studio_EventDescription_IsDopplerEnabled, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_BOOL*);
    errCheck(FMOD_Studio_EventDescription_IsDopplerEnabled(eventdescription, &doppler));
    FMODExt_push_FMOD_BOOL(L, doppler);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_HasSustainPoint(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output FMOD_BOOL* sustainPoint, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_HasSustainPoint
#define FMODExt_func_FMOD_Studio_EventDescription_HasSustainPoint _FMODExt_func_FMOD_Studio_EventDescription_HasSustainPoint
static int _FMODExt_func_FMOD_Studio_EventDescription_HasSustainPoint(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_BOOL sustainPoint;
    ensure(ST, FMOD_Studio_EventDescription_HasSustainPoint, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_BOOL*);
    errCheck(FMOD_Studio_EventDescription_HasSustainPoint(eventdescription, &sustainPoint));
    FMODExt_push_FMOD_BOOL(L, sustainPoint);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_CreateInstance(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output FMOD_STUDIO_EVENTINSTANCE** instance, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_CreateInstance
#define FMODExt_func_FMOD_Studio_EventDescription_CreateInstance _FMODExt_func_FMOD_Studio_EventDescription_CreateInstance
static int _FMODExt_func_FMOD_Studio_EventDescription_CreateInstance(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_STUDIO_EVENTINSTANCE* instance;
    ensure(ST, FMOD_Studio_EventDescription_CreateInstance, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_STUDIO_EVENTINSTANCE**);
    errCheck(FMOD_Studio_EventDescription_CreateInstance(eventdescription, &instance));
    FMODExt_push_ptr_FMOD_STUDIO_EVENTINSTANCE(L, instance);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetInstanceCount(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetInstanceCount
#define FMODExt_func_FMOD_Studio_EventDescription_GetInstanceCount _FMODExt_func_FMOD_Studio_EventDescription_GetInstanceCount
static int _FMODExt_func_FMOD_Studio_EventDescription_GetInstanceCount(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    int count;
    ensure(ST, FMOD_Studio_EventDescription_GetInstanceCount, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, int*);
    errCheck(FMOD_Studio_EventDescription_GetInstanceCount(eventdescription, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_GetInstanceList(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output FMOD_STUDIO_EVENTINSTANCE** array, input int capacity, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetInstanceList
#define FMODExt_func_FMOD_Studio_EventDescription_GetInstanceList _FMODExt_func_FMOD_Studio_EventDescription_GetInstanceList
static int _FMODExt_func_FMOD_Studio_EventDescription_GetInstanceList(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_STUDIO_EVENTINSTANCE* array;
    int capacity = FMODExt_check_int(L, 2);
    int count;
    ensure(ST, FMOD_Studio_EventDescription_GetInstanceList, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_STUDIO_EVENTINSTANCE**, int, int*);
    errCheck(FMOD_Studio_EventDescription_GetInstanceList(eventdescription, &array, capacity, &count));
    FMODExt_push_ptr_FMOD_STUDIO_EVENTINSTANCE(L, array);
    FMODExt_push_int(L, count);
    return 2;
}
#endif


/* FMOD_Studio_EventDescription_LoadSampleData(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_LoadSampleData
#define FMODExt_func_FMOD_Studio_EventDescription_LoadSampleData _FMODExt_func_FMOD_Studio_EventDescription_LoadSampleData
static int _FMODExt_func_FMOD_Studio_EventDescription_LoadSampleData(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    ensure(ST, FMOD_Studio_EventDescription_LoadSampleData, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*);
    errCheck(FMOD_Studio_EventDescription_LoadSampleData(eventdescription));
    return 0;
}
#endif


/* FMOD_Studio_EventDescription_UnloadSampleData(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_UnloadSampleData
#define FMODExt_func_FMOD_Studio_EventDescription_UnloadSampleData _FMODExt_func_FMOD_Studio_EventDescription_UnloadSampleData
static int _FMODExt_func_FMOD_Studio_EventDescription_UnloadSampleData(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    ensure(ST, FMOD_Studio_EventDescription_UnloadSampleData, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*);
    errCheck(FMOD_Studio_EventDescription_UnloadSampleData(eventdescription));
    return 0;
}
#endif


/* FMOD_Studio_EventDescription_GetSampleLoadingState(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, output FMOD_STUDIO_LOADING_STATE* state, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_GetSampleLoadingState
#define FMODExt_func_FMOD_Studio_EventDescription_GetSampleLoadingState _FMODExt_func_FMOD_Studio_EventDescription_GetSampleLoadingState
static int _FMODExt_func_FMOD_Studio_EventDescription_GetSampleLoadingState(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    FMOD_STUDIO_LOADING_STATE state;
    ensure(ST, FMOD_Studio_EventDescription_GetSampleLoadingState, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*, FMOD_STUDIO_LOADING_STATE*);
    errCheck(FMOD_Studio_EventDescription_GetSampleLoadingState(eventdescription, &state));
    FMODExt_push_FMOD_STUDIO_LOADING_STATE(L, state);
    return 1;
}
#endif


/* FMOD_Studio_EventDescription_ReleaseAllInstances(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, ) */
#ifndef FMODExt_func_FMOD_Studio_EventDescription_ReleaseAllInstances
#define FMODExt_func_FMOD_Studio_EventDescription_ReleaseAllInstances _FMODExt_func_FMOD_Studio_EventDescription_ReleaseAllInstances
static int _FMODExt_func_FMOD_Studio_EventDescription_ReleaseAllInstances(lua_State *L) {
    FMOD_STUDIO_EVENTDESCRIPTION* eventdescription = FMODExt_check_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, 1);
    ensure(ST, FMOD_Studio_EventDescription_ReleaseAllInstances, FMOD_RESULT, FMOD_STUDIO_EVENTDESCRIPTION*);
    errCheck(FMOD_Studio_EventDescription_ReleaseAllInstances(eventdescription));
    return 0;
}
#endif


/* FMOD_Studio_EventDescription_SetCallback(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, unknown FMOD_STUDIO_EVENT_CALLBACK callback, input FMOD_STUDIO_EVENT_CALLBACK_TYPE callbackmask, ) */


/* FMOD_Studio_EventDescription_GetUserData(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, unknown void** userdata, ) */


/* FMOD_Studio_EventDescription_SetUserData(input FMOD_STUDIO_EVENTDESCRIPTION* eventdescription, unknown void* userdata, ) */


/* FMOD_Studio_EventInstance_IsValid(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_IsValid
#define FMODExt_func_FMOD_Studio_EventInstance_IsValid _FMODExt_func_FMOD_Studio_EventInstance_IsValid
static int _FMODExt_func_FMOD_Studio_EventInstance_IsValid(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    ensure(ST, FMOD_Studio_EventInstance_IsValid, FMOD_BOOL, FMOD_STUDIO_EVENTINSTANCE*);
    FMOD_BOOL retval = FMOD_Studio_EventInstance_IsValid(eventinstance);
    lua_pushboolean(L, retval);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_GetDescription(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output FMOD_STUDIO_EVENTDESCRIPTION** description, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetDescription
#define FMODExt_func_FMOD_Studio_EventInstance_GetDescription _FMODExt_func_FMOD_Studio_EventInstance_GetDescription
static int _FMODExt_func_FMOD_Studio_EventInstance_GetDescription(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_EVENTDESCRIPTION* description;
    ensure(ST, FMOD_Studio_EventInstance_GetDescription, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_EVENTDESCRIPTION**);
    errCheck(FMOD_Studio_EventInstance_GetDescription(eventinstance, &description));
    FMODExt_push_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, description);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_GetSystem(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output FMOD_STUDIO_SYSTEM** system, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetSystem
#define FMODExt_func_FMOD_Studio_EventInstance_GetSystem _FMODExt_func_FMOD_Studio_EventInstance_GetSystem
static int _FMODExt_func_FMOD_Studio_EventInstance_GetSystem(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_SYSTEM* system;
    ensure(ST, FMOD_Studio_EventInstance_GetSystem, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_SYSTEM**);
    errCheck(FMOD_Studio_EventInstance_GetSystem(eventinstance, &system));
    FMODExt_push_ptr_FMOD_STUDIO_SYSTEM(L, system);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_GetVolume(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output float* volume, output float* finalvolume, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetVolume
#define FMODExt_func_FMOD_Studio_EventInstance_GetVolume _FMODExt_func_FMOD_Studio_EventInstance_GetVolume
static int _FMODExt_func_FMOD_Studio_EventInstance_GetVolume(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    float volume;
    float finalvolume;
    ensure(ST, FMOD_Studio_EventInstance_GetVolume, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, float*, float*);
    errCheck(FMOD_Studio_EventInstance_GetVolume(eventinstance, &volume, &finalvolume));
    FMODExt_push_float(L, volume);
    FMODExt_push_float(L, finalvolume);
    return 2;
}
#endif


/* FMOD_Studio_EventInstance_SetVolume(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input float volume, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetVolume
#define FMODExt_func_FMOD_Studio_EventInstance_SetVolume _FMODExt_func_FMOD_Studio_EventInstance_SetVolume
static int _FMODExt_func_FMOD_Studio_EventInstance_SetVolume(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    float volume = FMODExt_check_float(L, 2);
    ensure(ST, FMOD_Studio_EventInstance_SetVolume, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, float);
    errCheck(FMOD_Studio_EventInstance_SetVolume(eventinstance, volume));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_GetPitch(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output float* pitch, output float* finalpitch, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetPitch
#define FMODExt_func_FMOD_Studio_EventInstance_GetPitch _FMODExt_func_FMOD_Studio_EventInstance_GetPitch
static int _FMODExt_func_FMOD_Studio_EventInstance_GetPitch(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    float pitch;
    float finalpitch;
    ensure(ST, FMOD_Studio_EventInstance_GetPitch, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, float*, float*);
    errCheck(FMOD_Studio_EventInstance_GetPitch(eventinstance, &pitch, &finalpitch));
    FMODExt_push_float(L, pitch);
    FMODExt_push_float(L, finalpitch);
    return 2;
}
#endif


/* FMOD_Studio_EventInstance_SetPitch(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input float pitch, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetPitch
#define FMODExt_func_FMOD_Studio_EventInstance_SetPitch _FMODExt_func_FMOD_Studio_EventInstance_SetPitch
static int _FMODExt_func_FMOD_Studio_EventInstance_SetPitch(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    float pitch = FMODExt_check_float(L, 2);
    ensure(ST, FMOD_Studio_EventInstance_SetPitch, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, float);
    errCheck(FMOD_Studio_EventInstance_SetPitch(eventinstance, pitch));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_Get3DAttributes(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output_ptr FMOD_3D_ATTRIBUTES* attributes, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_Get3DAttributes
#define FMODExt_func_FMOD_Studio_EventInstance_Get3DAttributes _FMODExt_func_FMOD_Studio_EventInstance_Get3DAttributes
static int _FMODExt_func_FMOD_Studio_EventInstance_Get3DAttributes(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_3D_ATTRIBUTES* attributes = FMODExt_push_ptr_FMOD_3D_ATTRIBUTES(L, NULL);
    ensure(ST, FMOD_Studio_EventInstance_Get3DAttributes, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_3D_ATTRIBUTES*);
    errCheck(FMOD_Studio_EventInstance_Get3DAttributes(eventinstance, attributes));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_Set3DAttributes(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output_ptr FMOD_3D_ATTRIBUTES* attributes, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes
#define FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes _FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes
static int _FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_3D_ATTRIBUTES* attributes = FMODExt_push_ptr_FMOD_3D_ATTRIBUTES(L, NULL);
    ensure(ST, FMOD_Studio_EventInstance_Set3DAttributes, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_3D_ATTRIBUTES*);
    errCheck(FMOD_Studio_EventInstance_Set3DAttributes(eventinstance, attributes));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_GetListenerMask(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output unsigned int* mask, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetListenerMask
#define FMODExt_func_FMOD_Studio_EventInstance_GetListenerMask _FMODExt_func_FMOD_Studio_EventInstance_GetListenerMask
static int _FMODExt_func_FMOD_Studio_EventInstance_GetListenerMask(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    unsigned int mask;
    ensure(ST, FMOD_Studio_EventInstance_GetListenerMask, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, unsigned int*);
    errCheck(FMOD_Studio_EventInstance_GetListenerMask(eventinstance, &mask));
    FMODExt_push_unsigned_int(L, mask);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_SetListenerMask(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input unsigned int mask, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetListenerMask
#define FMODExt_func_FMOD_Studio_EventInstance_SetListenerMask _FMODExt_func_FMOD_Studio_EventInstance_SetListenerMask
static int _FMODExt_func_FMOD_Studio_EventInstance_SetListenerMask(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    unsigned int mask = FMODExt_check_unsigned_int(L, 2);
    ensure(ST, FMOD_Studio_EventInstance_SetListenerMask, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, unsigned int);
    errCheck(FMOD_Studio_EventInstance_SetListenerMask(eventinstance, mask));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_GetProperty(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input FMOD_STUDIO_EVENT_PROPERTY index, output float* value, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetProperty
#define FMODExt_func_FMOD_Studio_EventInstance_GetProperty _FMODExt_func_FMOD_Studio_EventInstance_GetProperty
static int _FMODExt_func_FMOD_Studio_EventInstance_GetProperty(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_EVENT_PROPERTY index = FMODExt_check_FMOD_STUDIO_EVENT_PROPERTY(L, 2);
    float value;
    ensure(ST, FMOD_Studio_EventInstance_GetProperty, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_EVENT_PROPERTY, float*);
    errCheck(FMOD_Studio_EventInstance_GetProperty(eventinstance, index, &value));
    FMODExt_push_float(L, value);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_SetProperty(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input FMOD_STUDIO_EVENT_PROPERTY index, input float value, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetProperty
#define FMODExt_func_FMOD_Studio_EventInstance_SetProperty _FMODExt_func_FMOD_Studio_EventInstance_SetProperty
static int _FMODExt_func_FMOD_Studio_EventInstance_SetProperty(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_EVENT_PROPERTY index = FMODExt_check_FMOD_STUDIO_EVENT_PROPERTY(L, 2);
    float value = FMODExt_check_float(L, 3);
    ensure(ST, FMOD_Studio_EventInstance_SetProperty, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_EVENT_PROPERTY, float);
    errCheck(FMOD_Studio_EventInstance_SetProperty(eventinstance, index, value));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_GetReverbLevel(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input int index, output float* level, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetReverbLevel
#define FMODExt_func_FMOD_Studio_EventInstance_GetReverbLevel _FMODExt_func_FMOD_Studio_EventInstance_GetReverbLevel
static int _FMODExt_func_FMOD_Studio_EventInstance_GetReverbLevel(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    int index = FMODExt_check_int(L, 2);
    float level;
    ensure(ST, FMOD_Studio_EventInstance_GetReverbLevel, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, int, float*);
    errCheck(FMOD_Studio_EventInstance_GetReverbLevel(eventinstance, index, &level));
    FMODExt_push_float(L, level);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_SetReverbLevel(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input int index, input float level, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetReverbLevel
#define FMODExt_func_FMOD_Studio_EventInstance_SetReverbLevel _FMODExt_func_FMOD_Studio_EventInstance_SetReverbLevel
static int _FMODExt_func_FMOD_Studio_EventInstance_SetReverbLevel(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    int index = FMODExt_check_int(L, 2);
    float level = FMODExt_check_float(L, 3);
    ensure(ST, FMOD_Studio_EventInstance_SetReverbLevel, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, int, float);
    errCheck(FMOD_Studio_EventInstance_SetReverbLevel(eventinstance, index, level));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_GetPaused(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output FMOD_BOOL* paused, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetPaused
#define FMODExt_func_FMOD_Studio_EventInstance_GetPaused _FMODExt_func_FMOD_Studio_EventInstance_GetPaused
static int _FMODExt_func_FMOD_Studio_EventInstance_GetPaused(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_BOOL paused;
    ensure(ST, FMOD_Studio_EventInstance_GetPaused, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_BOOL*);
    errCheck(FMOD_Studio_EventInstance_GetPaused(eventinstance, &paused));
    FMODExt_push_FMOD_BOOL(L, paused);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_SetPaused(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input FMOD_BOOL paused, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetPaused
#define FMODExt_func_FMOD_Studio_EventInstance_SetPaused _FMODExt_func_FMOD_Studio_EventInstance_SetPaused
static int _FMODExt_func_FMOD_Studio_EventInstance_SetPaused(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_BOOL paused = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(ST, FMOD_Studio_EventInstance_SetPaused, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_BOOL);
    errCheck(FMOD_Studio_EventInstance_SetPaused(eventinstance, paused));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_Start(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_Start
#define FMODExt_func_FMOD_Studio_EventInstance_Start _FMODExt_func_FMOD_Studio_EventInstance_Start
static int _FMODExt_func_FMOD_Studio_EventInstance_Start(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    ensure(ST, FMOD_Studio_EventInstance_Start, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*);
    errCheck(FMOD_Studio_EventInstance_Start(eventinstance));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_Stop(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input FMOD_STUDIO_STOP_MODE mode, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_Stop
#define FMODExt_func_FMOD_Studio_EventInstance_Stop _FMODExt_func_FMOD_Studio_EventInstance_Stop
static int _FMODExt_func_FMOD_Studio_EventInstance_Stop(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_STOP_MODE mode = FMODExt_check_FMOD_STUDIO_STOP_MODE(L, 2);
    ensure(ST, FMOD_Studio_EventInstance_Stop, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_STOP_MODE);
    errCheck(FMOD_Studio_EventInstance_Stop(eventinstance, mode));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_GetTimelinePosition(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output int* position, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetTimelinePosition
#define FMODExt_func_FMOD_Studio_EventInstance_GetTimelinePosition _FMODExt_func_FMOD_Studio_EventInstance_GetTimelinePosition
static int _FMODExt_func_FMOD_Studio_EventInstance_GetTimelinePosition(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    int position;
    ensure(ST, FMOD_Studio_EventInstance_GetTimelinePosition, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, int*);
    errCheck(FMOD_Studio_EventInstance_GetTimelinePosition(eventinstance, &position));
    FMODExt_push_int(L, position);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_SetTimelinePosition(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input int position, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetTimelinePosition
#define FMODExt_func_FMOD_Studio_EventInstance_SetTimelinePosition _FMODExt_func_FMOD_Studio_EventInstance_SetTimelinePosition
static int _FMODExt_func_FMOD_Studio_EventInstance_SetTimelinePosition(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    int position = FMODExt_check_int(L, 2);
    ensure(ST, FMOD_Studio_EventInstance_SetTimelinePosition, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, int);
    errCheck(FMOD_Studio_EventInstance_SetTimelinePosition(eventinstance, position));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_GetPlaybackState(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output FMOD_STUDIO_PLAYBACK_STATE* state, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetPlaybackState
#define FMODExt_func_FMOD_Studio_EventInstance_GetPlaybackState _FMODExt_func_FMOD_Studio_EventInstance_GetPlaybackState
static int _FMODExt_func_FMOD_Studio_EventInstance_GetPlaybackState(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_PLAYBACK_STATE state;
    ensure(ST, FMOD_Studio_EventInstance_GetPlaybackState, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_PLAYBACK_STATE*);
    errCheck(FMOD_Studio_EventInstance_GetPlaybackState(eventinstance, &state));
    FMODExt_push_FMOD_STUDIO_PLAYBACK_STATE(L, state);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_GetChannelGroup(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output FMOD_CHANNELGROUP** group, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetChannelGroup
#define FMODExt_func_FMOD_Studio_EventInstance_GetChannelGroup _FMODExt_func_FMOD_Studio_EventInstance_GetChannelGroup
static int _FMODExt_func_FMOD_Studio_EventInstance_GetChannelGroup(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_CHANNELGROUP* group;
    ensure(ST, FMOD_Studio_EventInstance_GetChannelGroup, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_CHANNELGROUP**);
    errCheck(FMOD_Studio_EventInstance_GetChannelGroup(eventinstance, &group));
    FMODExt_push_ptr_FMOD_CHANNELGROUP(L, group);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_GetMinMaxDistance(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output float* min, output float* max, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetMinMaxDistance
#define FMODExt_func_FMOD_Studio_EventInstance_GetMinMaxDistance _FMODExt_func_FMOD_Studio_EventInstance_GetMinMaxDistance
static int _FMODExt_func_FMOD_Studio_EventInstance_GetMinMaxDistance(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    float min;
    float max;
    ensure(ST, FMOD_Studio_EventInstance_GetMinMaxDistance, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, float*, float*);
    errCheck(FMOD_Studio_EventInstance_GetMinMaxDistance(eventinstance, &min, &max));
    FMODExt_push_float(L, min);
    FMODExt_push_float(L, max);
    return 2;
}
#endif


/* FMOD_Studio_EventInstance_Release(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_Release
#define FMODExt_func_FMOD_Studio_EventInstance_Release _FMODExt_func_FMOD_Studio_EventInstance_Release
static int _FMODExt_func_FMOD_Studio_EventInstance_Release(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    ensure(ST, FMOD_Studio_EventInstance_Release, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*);
    errCheck(FMOD_Studio_EventInstance_Release(eventinstance));
    
    lua_rawgeti(L, LUA_REGISTRYINDEX, FMODExt_registry_refcount);
    lua_pushlightuserdata(L, eventinstance);
    lua_pushnil(L);
    lua_rawset(L, -3);
    lua_pop(L, 1);
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_IsVirtual(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output FMOD_BOOL* virtualstate, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_IsVirtual
#define FMODExt_func_FMOD_Studio_EventInstance_IsVirtual _FMODExt_func_FMOD_Studio_EventInstance_IsVirtual
static int _FMODExt_func_FMOD_Studio_EventInstance_IsVirtual(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_BOOL virtualstate;
    ensure(ST, FMOD_Studio_EventInstance_IsVirtual, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_BOOL*);
    errCheck(FMOD_Studio_EventInstance_IsVirtual(eventinstance, &virtualstate));
    FMODExt_push_FMOD_BOOL(L, virtualstate);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_GetParameterByName(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input const char* name, output float* value, output float* finalvalue, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetParameterByName
#define FMODExt_func_FMOD_Studio_EventInstance_GetParameterByName _FMODExt_func_FMOD_Studio_EventInstance_GetParameterByName
static int _FMODExt_func_FMOD_Studio_EventInstance_GetParameterByName(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    float value;
    float finalvalue;
    ensure(ST, FMOD_Studio_EventInstance_GetParameterByName, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, const char*, float*, float*);
    errCheck(FMOD_Studio_EventInstance_GetParameterByName(eventinstance, name, &value, &finalvalue));
    FMODExt_push_float(L, value);
    FMODExt_push_float(L, finalvalue);
    return 2;
}
#endif


/* FMOD_Studio_EventInstance_SetParameterByName(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input const char* name, input float value, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetParameterByName
#define FMODExt_func_FMOD_Studio_EventInstance_SetParameterByName _FMODExt_func_FMOD_Studio_EventInstance_SetParameterByName
static int _FMODExt_func_FMOD_Studio_EventInstance_SetParameterByName(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    float value = FMODExt_check_float(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_EventInstance_SetParameterByName, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, const char*, float, FMOD_BOOL);
    errCheck(FMOD_Studio_EventInstance_SetParameterByName(eventinstance, name, value, ignoreseekspeed));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_SetParameterByNameWithLabel(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input const char* name, input const char* label, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetParameterByNameWithLabel
#define FMODExt_func_FMOD_Studio_EventInstance_SetParameterByNameWithLabel _FMODExt_func_FMOD_Studio_EventInstance_SetParameterByNameWithLabel
static int _FMODExt_func_FMOD_Studio_EventInstance_SetParameterByNameWithLabel(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    const char* name = FMODExt_check_ptr_char(L, 2);
    const char* label = FMODExt_check_ptr_char(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_EventInstance_SetParameterByNameWithLabel, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, const char*, const char*, FMOD_BOOL);
    errCheck(FMOD_Studio_EventInstance_SetParameterByNameWithLabel(eventinstance, name, label, ignoreseekspeed));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_GetParameterByID(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input_deref FMOD_STUDIO_PARAMETER_ID id, output float* value, output float* finalvalue, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetParameterByID
#define FMODExt_func_FMOD_Studio_EventInstance_GetParameterByID _FMODExt_func_FMOD_Studio_EventInstance_GetParameterByID
static int _FMODExt_func_FMOD_Studio_EventInstance_GetParameterByID(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_PARAMETER_ID* id = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    float value;
    float finalvalue;
    ensure(ST, FMOD_Studio_EventInstance_GetParameterByID, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_PARAMETER_ID, float*, float*);
    errCheck(FMOD_Studio_EventInstance_GetParameterByID(eventinstance, *id, &value, &finalvalue));
    FMODExt_push_float(L, value);
    FMODExt_push_float(L, finalvalue);
    return 2;
}
#endif


/* FMOD_Studio_EventInstance_SetParameterByID(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input_deref FMOD_STUDIO_PARAMETER_ID id, input float value, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetParameterByID
#define FMODExt_func_FMOD_Studio_EventInstance_SetParameterByID _FMODExt_func_FMOD_Studio_EventInstance_SetParameterByID
static int _FMODExt_func_FMOD_Studio_EventInstance_SetParameterByID(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_PARAMETER_ID* id = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    float value = FMODExt_check_float(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_EventInstance_SetParameterByID, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_PARAMETER_ID, float, FMOD_BOOL);
    errCheck(FMOD_Studio_EventInstance_SetParameterByID(eventinstance, *id, value, ignoreseekspeed));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_SetParameterByIDWithLabel(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input_deref FMOD_STUDIO_PARAMETER_ID id, input const char* label, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetParameterByIDWithLabel
#define FMODExt_func_FMOD_Studio_EventInstance_SetParameterByIDWithLabel _FMODExt_func_FMOD_Studio_EventInstance_SetParameterByIDWithLabel
static int _FMODExt_func_FMOD_Studio_EventInstance_SetParameterByIDWithLabel(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_PARAMETER_ID* id = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    const char* label = FMODExt_check_ptr_char(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_EventInstance_SetParameterByIDWithLabel, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_PARAMETER_ID, const char*, FMOD_BOOL);
    errCheck(FMOD_Studio_EventInstance_SetParameterByIDWithLabel(eventinstance, *id, label, ignoreseekspeed));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_SetParametersByIDs(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, input const FMOD_STUDIO_PARAMETER_ID* ids, output float* values, input int count, input FMOD_BOOL ignoreseekspeed, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_SetParametersByIDs
#define FMODExt_func_FMOD_Studio_EventInstance_SetParametersByIDs _FMODExt_func_FMOD_Studio_EventInstance_SetParametersByIDs
static int _FMODExt_func_FMOD_Studio_EventInstance_SetParametersByIDs(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    const FMOD_STUDIO_PARAMETER_ID* ids = FMODExt_check_ptr_FMOD_STUDIO_PARAMETER_ID(L, 2);
    float values;
    int count = FMODExt_check_int(L, 3);
    FMOD_BOOL ignoreseekspeed = FMODExt_check_FMOD_BOOL(L, 4);
    ensure(ST, FMOD_Studio_EventInstance_SetParametersByIDs, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, const FMOD_STUDIO_PARAMETER_ID*, float*, int, FMOD_BOOL);
    errCheck(FMOD_Studio_EventInstance_SetParametersByIDs(eventinstance, ids, &values, count, ignoreseekspeed));
    FMODExt_push_float(L, values);
    return 1;
}
#endif


/* FMOD_Studio_EventInstance_KeyOff(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_KeyOff
#define FMODExt_func_FMOD_Studio_EventInstance_KeyOff _FMODExt_func_FMOD_Studio_EventInstance_KeyOff
static int _FMODExt_func_FMOD_Studio_EventInstance_KeyOff(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    ensure(ST, FMOD_Studio_EventInstance_KeyOff, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*);
    errCheck(FMOD_Studio_EventInstance_KeyOff(eventinstance));
    return 0;
}
#endif


/* FMOD_Studio_EventInstance_SetCallback(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, unknown FMOD_STUDIO_EVENT_CALLBACK callback, input FMOD_STUDIO_EVENT_CALLBACK_TYPE callbackmask, ) */


/* FMOD_Studio_EventInstance_GetUserData(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, unknown void** userdata, ) */


/* FMOD_Studio_EventInstance_SetUserData(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, unknown void* userdata, ) */


/* FMOD_Studio_EventInstance_GetCPUUsage(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output unsigned int* exclusive, output unsigned int* inclusive, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetCPUUsage
#define FMODExt_func_FMOD_Studio_EventInstance_GetCPUUsage _FMODExt_func_FMOD_Studio_EventInstance_GetCPUUsage
static int _FMODExt_func_FMOD_Studio_EventInstance_GetCPUUsage(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    unsigned int exclusive;
    unsigned int inclusive;
    ensure(ST, FMOD_Studio_EventInstance_GetCPUUsage, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, unsigned int*, unsigned int*);
    errCheck(FMOD_Studio_EventInstance_GetCPUUsage(eventinstance, &exclusive, &inclusive));
    FMODExt_push_unsigned_int(L, exclusive);
    FMODExt_push_unsigned_int(L, inclusive);
    return 2;
}
#endif


/* FMOD_Studio_EventInstance_GetMemoryUsage(input FMOD_STUDIO_EVENTINSTANCE* eventinstance, output_ptr FMOD_STUDIO_MEMORY_USAGE* memoryusage, ) */
#ifndef FMODExt_func_FMOD_Studio_EventInstance_GetMemoryUsage
#define FMODExt_func_FMOD_Studio_EventInstance_GetMemoryUsage _FMODExt_func_FMOD_Studio_EventInstance_GetMemoryUsage
static int _FMODExt_func_FMOD_Studio_EventInstance_GetMemoryUsage(lua_State *L) {
    FMOD_STUDIO_EVENTINSTANCE* eventinstance = FMODExt_check_ptr_FMOD_STUDIO_EVENTINSTANCE(L, 1);
    FMOD_STUDIO_MEMORY_USAGE* memoryusage = FMODExt_push_ptr_FMOD_STUDIO_MEMORY_USAGE(L, NULL);
    ensure(ST, FMOD_Studio_EventInstance_GetMemoryUsage, FMOD_RESULT, FMOD_STUDIO_EVENTINSTANCE*, FMOD_STUDIO_MEMORY_USAGE*);
    errCheck(FMOD_Studio_EventInstance_GetMemoryUsage(eventinstance, memoryusage));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_Bus_IsValid(input FMOD_STUDIO_BUS* bus, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_IsValid
#define FMODExt_func_FMOD_Studio_Bus_IsValid _FMODExt_func_FMOD_Studio_Bus_IsValid
static int _FMODExt_func_FMOD_Studio_Bus_IsValid(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    ensure(ST, FMOD_Studio_Bus_IsValid, FMOD_BOOL, FMOD_STUDIO_BUS*);
    FMOD_BOOL retval = FMOD_Studio_Bus_IsValid(bus);
    lua_pushboolean(L, retval);
    return 1;
}
#endif


/* FMOD_Studio_Bus_GetID(input FMOD_STUDIO_BUS* bus, output_ptr FMOD_GUID* id, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_GetID
#define FMODExt_func_FMOD_Studio_Bus_GetID _FMODExt_func_FMOD_Studio_Bus_GetID
static int _FMODExt_func_FMOD_Studio_Bus_GetID(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_GUID* id = FMODExt_push_ptr_FMOD_GUID(L, NULL);
    ensure(ST, FMOD_Studio_Bus_GetID, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_GUID*);
    errCheck(FMOD_Studio_Bus_GetID(bus, id));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_Bus_GetPath(input FMOD_STUDIO_BUS* bus, unknown char* path, input int size, output int* retrieved, ) */


/* FMOD_Studio_Bus_GetVolume(input FMOD_STUDIO_BUS* bus, output float* volume, output float* finalvolume, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_GetVolume
#define FMODExt_func_FMOD_Studio_Bus_GetVolume _FMODExt_func_FMOD_Studio_Bus_GetVolume
static int _FMODExt_func_FMOD_Studio_Bus_GetVolume(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    float volume;
    float finalvolume;
    ensure(ST, FMOD_Studio_Bus_GetVolume, FMOD_RESULT, FMOD_STUDIO_BUS*, float*, float*);
    errCheck(FMOD_Studio_Bus_GetVolume(bus, &volume, &finalvolume));
    FMODExt_push_float(L, volume);
    FMODExt_push_float(L, finalvolume);
    return 2;
}
#endif


/* FMOD_Studio_Bus_SetVolume(input FMOD_STUDIO_BUS* bus, input float volume, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_SetVolume
#define FMODExt_func_FMOD_Studio_Bus_SetVolume _FMODExt_func_FMOD_Studio_Bus_SetVolume
static int _FMODExt_func_FMOD_Studio_Bus_SetVolume(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    float volume = FMODExt_check_float(L, 2);
    ensure(ST, FMOD_Studio_Bus_SetVolume, FMOD_RESULT, FMOD_STUDIO_BUS*, float);
    errCheck(FMOD_Studio_Bus_SetVolume(bus, volume));
    return 0;
}
#endif


/* FMOD_Studio_Bus_GetPaused(input FMOD_STUDIO_BUS* bus, output FMOD_BOOL* paused, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_GetPaused
#define FMODExt_func_FMOD_Studio_Bus_GetPaused _FMODExt_func_FMOD_Studio_Bus_GetPaused
static int _FMODExt_func_FMOD_Studio_Bus_GetPaused(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_BOOL paused;
    ensure(ST, FMOD_Studio_Bus_GetPaused, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_BOOL*);
    errCheck(FMOD_Studio_Bus_GetPaused(bus, &paused));
    FMODExt_push_FMOD_BOOL(L, paused);
    return 1;
}
#endif


/* FMOD_Studio_Bus_SetPaused(input FMOD_STUDIO_BUS* bus, input FMOD_BOOL paused, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_SetPaused
#define FMODExt_func_FMOD_Studio_Bus_SetPaused _FMODExt_func_FMOD_Studio_Bus_SetPaused
static int _FMODExt_func_FMOD_Studio_Bus_SetPaused(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_BOOL paused = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(ST, FMOD_Studio_Bus_SetPaused, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_BOOL);
    errCheck(FMOD_Studio_Bus_SetPaused(bus, paused));
    return 0;
}
#endif


/* FMOD_Studio_Bus_GetMute(input FMOD_STUDIO_BUS* bus, output FMOD_BOOL* mute, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_GetMute
#define FMODExt_func_FMOD_Studio_Bus_GetMute _FMODExt_func_FMOD_Studio_Bus_GetMute
static int _FMODExt_func_FMOD_Studio_Bus_GetMute(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_BOOL mute;
    ensure(ST, FMOD_Studio_Bus_GetMute, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_BOOL*);
    errCheck(FMOD_Studio_Bus_GetMute(bus, &mute));
    FMODExt_push_FMOD_BOOL(L, mute);
    return 1;
}
#endif


/* FMOD_Studio_Bus_SetMute(input FMOD_STUDIO_BUS* bus, input FMOD_BOOL mute, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_SetMute
#define FMODExt_func_FMOD_Studio_Bus_SetMute _FMODExt_func_FMOD_Studio_Bus_SetMute
static int _FMODExt_func_FMOD_Studio_Bus_SetMute(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_BOOL mute = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(ST, FMOD_Studio_Bus_SetMute, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_BOOL);
    errCheck(FMOD_Studio_Bus_SetMute(bus, mute));
    return 0;
}
#endif


/* FMOD_Studio_Bus_StopAllEvents(input FMOD_STUDIO_BUS* bus, input FMOD_STUDIO_STOP_MODE mode, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_StopAllEvents
#define FMODExt_func_FMOD_Studio_Bus_StopAllEvents _FMODExt_func_FMOD_Studio_Bus_StopAllEvents
static int _FMODExt_func_FMOD_Studio_Bus_StopAllEvents(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_STUDIO_STOP_MODE mode = FMODExt_check_FMOD_STUDIO_STOP_MODE(L, 2);
    ensure(ST, FMOD_Studio_Bus_StopAllEvents, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_STUDIO_STOP_MODE);
    errCheck(FMOD_Studio_Bus_StopAllEvents(bus, mode));
    return 0;
}
#endif


/* FMOD_Studio_Bus_GetPortIndex(input FMOD_STUDIO_BUS* bus, output FMOD_PORT_INDEX* index, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_GetPortIndex
#define FMODExt_func_FMOD_Studio_Bus_GetPortIndex _FMODExt_func_FMOD_Studio_Bus_GetPortIndex
static int _FMODExt_func_FMOD_Studio_Bus_GetPortIndex(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_PORT_INDEX index;
    ensure(ST, FMOD_Studio_Bus_GetPortIndex, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_PORT_INDEX*);
    errCheck(FMOD_Studio_Bus_GetPortIndex(bus, &index));
    FMODExt_push_unsigned_long_long(L, index);
    return 1;
}
#endif


/* FMOD_Studio_Bus_SetPortIndex(input FMOD_STUDIO_BUS* bus, input FMOD_PORT_INDEX index, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_SetPortIndex
#define FMODExt_func_FMOD_Studio_Bus_SetPortIndex _FMODExt_func_FMOD_Studio_Bus_SetPortIndex
static int _FMODExt_func_FMOD_Studio_Bus_SetPortIndex(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_PORT_INDEX index = FMODExt_check_unsigned_long_long(L, 2);
    ensure(ST, FMOD_Studio_Bus_SetPortIndex, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_PORT_INDEX);
    errCheck(FMOD_Studio_Bus_SetPortIndex(bus, index));
    return 0;
}
#endif


/* FMOD_Studio_Bus_LockChannelGroup(input FMOD_STUDIO_BUS* bus, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_LockChannelGroup
#define FMODExt_func_FMOD_Studio_Bus_LockChannelGroup _FMODExt_func_FMOD_Studio_Bus_LockChannelGroup
static int _FMODExt_func_FMOD_Studio_Bus_LockChannelGroup(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    ensure(ST, FMOD_Studio_Bus_LockChannelGroup, FMOD_RESULT, FMOD_STUDIO_BUS*);
    errCheck(FMOD_Studio_Bus_LockChannelGroup(bus));
    return 0;
}
#endif


/* FMOD_Studio_Bus_UnlockChannelGroup(input FMOD_STUDIO_BUS* bus, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_UnlockChannelGroup
#define FMODExt_func_FMOD_Studio_Bus_UnlockChannelGroup _FMODExt_func_FMOD_Studio_Bus_UnlockChannelGroup
static int _FMODExt_func_FMOD_Studio_Bus_UnlockChannelGroup(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    ensure(ST, FMOD_Studio_Bus_UnlockChannelGroup, FMOD_RESULT, FMOD_STUDIO_BUS*);
    errCheck(FMOD_Studio_Bus_UnlockChannelGroup(bus));
    return 0;
}
#endif


/* FMOD_Studio_Bus_GetChannelGroup(input FMOD_STUDIO_BUS* bus, output FMOD_CHANNELGROUP** group, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_GetChannelGroup
#define FMODExt_func_FMOD_Studio_Bus_GetChannelGroup _FMODExt_func_FMOD_Studio_Bus_GetChannelGroup
static int _FMODExt_func_FMOD_Studio_Bus_GetChannelGroup(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_CHANNELGROUP* group;
    ensure(ST, FMOD_Studio_Bus_GetChannelGroup, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_CHANNELGROUP**);
    errCheck(FMOD_Studio_Bus_GetChannelGroup(bus, &group));
    FMODExt_push_ptr_FMOD_CHANNELGROUP(L, group);
    return 1;
}
#endif


/* FMOD_Studio_Bus_GetCPUUsage(input FMOD_STUDIO_BUS* bus, output unsigned int* exclusive, output unsigned int* inclusive, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_GetCPUUsage
#define FMODExt_func_FMOD_Studio_Bus_GetCPUUsage _FMODExt_func_FMOD_Studio_Bus_GetCPUUsage
static int _FMODExt_func_FMOD_Studio_Bus_GetCPUUsage(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    unsigned int exclusive;
    unsigned int inclusive;
    ensure(ST, FMOD_Studio_Bus_GetCPUUsage, FMOD_RESULT, FMOD_STUDIO_BUS*, unsigned int*, unsigned int*);
    errCheck(FMOD_Studio_Bus_GetCPUUsage(bus, &exclusive, &inclusive));
    FMODExt_push_unsigned_int(L, exclusive);
    FMODExt_push_unsigned_int(L, inclusive);
    return 2;
}
#endif


/* FMOD_Studio_Bus_GetMemoryUsage(input FMOD_STUDIO_BUS* bus, output_ptr FMOD_STUDIO_MEMORY_USAGE* memoryusage, ) */
#ifndef FMODExt_func_FMOD_Studio_Bus_GetMemoryUsage
#define FMODExt_func_FMOD_Studio_Bus_GetMemoryUsage _FMODExt_func_FMOD_Studio_Bus_GetMemoryUsage
static int _FMODExt_func_FMOD_Studio_Bus_GetMemoryUsage(lua_State *L) {
    FMOD_STUDIO_BUS* bus = FMODExt_check_ptr_FMOD_STUDIO_BUS(L, 1);
    FMOD_STUDIO_MEMORY_USAGE* memoryusage = FMODExt_push_ptr_FMOD_STUDIO_MEMORY_USAGE(L, NULL);
    ensure(ST, FMOD_Studio_Bus_GetMemoryUsage, FMOD_RESULT, FMOD_STUDIO_BUS*, FMOD_STUDIO_MEMORY_USAGE*);
    errCheck(FMOD_Studio_Bus_GetMemoryUsage(bus, memoryusage));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_VCA_IsValid(input FMOD_STUDIO_VCA* vca, ) */
#ifndef FMODExt_func_FMOD_Studio_VCA_IsValid
#define FMODExt_func_FMOD_Studio_VCA_IsValid _FMODExt_func_FMOD_Studio_VCA_IsValid
static int _FMODExt_func_FMOD_Studio_VCA_IsValid(lua_State *L) {
    FMOD_STUDIO_VCA* vca = FMODExt_check_ptr_FMOD_STUDIO_VCA(L, 1);
    ensure(ST, FMOD_Studio_VCA_IsValid, FMOD_BOOL, FMOD_STUDIO_VCA*);
    FMOD_BOOL retval = FMOD_Studio_VCA_IsValid(vca);
    lua_pushboolean(L, retval);
    return 1;
}
#endif


/* FMOD_Studio_VCA_GetID(input FMOD_STUDIO_VCA* vca, output_ptr FMOD_GUID* id, ) */
#ifndef FMODExt_func_FMOD_Studio_VCA_GetID
#define FMODExt_func_FMOD_Studio_VCA_GetID _FMODExt_func_FMOD_Studio_VCA_GetID
static int _FMODExt_func_FMOD_Studio_VCA_GetID(lua_State *L) {
    FMOD_STUDIO_VCA* vca = FMODExt_check_ptr_FMOD_STUDIO_VCA(L, 1);
    FMOD_GUID* id = FMODExt_push_ptr_FMOD_GUID(L, NULL);
    ensure(ST, FMOD_Studio_VCA_GetID, FMOD_RESULT, FMOD_STUDIO_VCA*, FMOD_GUID*);
    errCheck(FMOD_Studio_VCA_GetID(vca, id));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_VCA_GetPath(input FMOD_STUDIO_VCA* vca, unknown char* path, input int size, output int* retrieved, ) */


/* FMOD_Studio_VCA_GetVolume(input FMOD_STUDIO_VCA* vca, output float* volume, output float* finalvolume, ) */
#ifndef FMODExt_func_FMOD_Studio_VCA_GetVolume
#define FMODExt_func_FMOD_Studio_VCA_GetVolume _FMODExt_func_FMOD_Studio_VCA_GetVolume
static int _FMODExt_func_FMOD_Studio_VCA_GetVolume(lua_State *L) {
    FMOD_STUDIO_VCA* vca = FMODExt_check_ptr_FMOD_STUDIO_VCA(L, 1);
    float volume;
    float finalvolume;
    ensure(ST, FMOD_Studio_VCA_GetVolume, FMOD_RESULT, FMOD_STUDIO_VCA*, float*, float*);
    errCheck(FMOD_Studio_VCA_GetVolume(vca, &volume, &finalvolume));
    FMODExt_push_float(L, volume);
    FMODExt_push_float(L, finalvolume);
    return 2;
}
#endif


/* FMOD_Studio_VCA_SetVolume(input FMOD_STUDIO_VCA* vca, input float volume, ) */
#ifndef FMODExt_func_FMOD_Studio_VCA_SetVolume
#define FMODExt_func_FMOD_Studio_VCA_SetVolume _FMODExt_func_FMOD_Studio_VCA_SetVolume
static int _FMODExt_func_FMOD_Studio_VCA_SetVolume(lua_State *L) {
    FMOD_STUDIO_VCA* vca = FMODExt_check_ptr_FMOD_STUDIO_VCA(L, 1);
    float volume = FMODExt_check_float(L, 2);
    ensure(ST, FMOD_Studio_VCA_SetVolume, FMOD_RESULT, FMOD_STUDIO_VCA*, float);
    errCheck(FMOD_Studio_VCA_SetVolume(vca, volume));
    return 0;
}
#endif


/* FMOD_Studio_Bank_IsValid(input FMOD_STUDIO_BANK* bank, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_IsValid
#define FMODExt_func_FMOD_Studio_Bank_IsValid _FMODExt_func_FMOD_Studio_Bank_IsValid
static int _FMODExt_func_FMOD_Studio_Bank_IsValid(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    ensure(ST, FMOD_Studio_Bank_IsValid, FMOD_BOOL, FMOD_STUDIO_BANK*);
    FMOD_BOOL retval = FMOD_Studio_Bank_IsValid(bank);
    lua_pushboolean(L, retval);
    return 1;
}
#endif


/* FMOD_Studio_Bank_GetID(input FMOD_STUDIO_BANK* bank, output_ptr FMOD_GUID* id, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetID
#define FMODExt_func_FMOD_Studio_Bank_GetID _FMODExt_func_FMOD_Studio_Bank_GetID
static int _FMODExt_func_FMOD_Studio_Bank_GetID(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    FMOD_GUID* id = FMODExt_push_ptr_FMOD_GUID(L, NULL);
    ensure(ST, FMOD_Studio_Bank_GetID, FMOD_RESULT, FMOD_STUDIO_BANK*, FMOD_GUID*);
    errCheck(FMOD_Studio_Bank_GetID(bank, id));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_Bank_GetPath(input FMOD_STUDIO_BANK* bank, unknown char* path, input int size, output int* retrieved, ) */


/* FMOD_Studio_Bank_Unload(input FMOD_STUDIO_BANK* bank, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_Unload
#define FMODExt_func_FMOD_Studio_Bank_Unload _FMODExt_func_FMOD_Studio_Bank_Unload
static int _FMODExt_func_FMOD_Studio_Bank_Unload(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    ensure(ST, FMOD_Studio_Bank_Unload, FMOD_RESULT, FMOD_STUDIO_BANK*);
    errCheck(FMOD_Studio_Bank_Unload(bank));
    return 0;
}
#endif


/* FMOD_Studio_Bank_LoadSampleData(input FMOD_STUDIO_BANK* bank, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_LoadSampleData
#define FMODExt_func_FMOD_Studio_Bank_LoadSampleData _FMODExt_func_FMOD_Studio_Bank_LoadSampleData
static int _FMODExt_func_FMOD_Studio_Bank_LoadSampleData(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    ensure(ST, FMOD_Studio_Bank_LoadSampleData, FMOD_RESULT, FMOD_STUDIO_BANK*);
    errCheck(FMOD_Studio_Bank_LoadSampleData(bank));
    return 0;
}
#endif


/* FMOD_Studio_Bank_UnloadSampleData(input FMOD_STUDIO_BANK* bank, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_UnloadSampleData
#define FMODExt_func_FMOD_Studio_Bank_UnloadSampleData _FMODExt_func_FMOD_Studio_Bank_UnloadSampleData
static int _FMODExt_func_FMOD_Studio_Bank_UnloadSampleData(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    ensure(ST, FMOD_Studio_Bank_UnloadSampleData, FMOD_RESULT, FMOD_STUDIO_BANK*);
    errCheck(FMOD_Studio_Bank_UnloadSampleData(bank));
    return 0;
}
#endif


/* FMOD_Studio_Bank_GetLoadingState(input FMOD_STUDIO_BANK* bank, output FMOD_STUDIO_LOADING_STATE* state, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetLoadingState
#define FMODExt_func_FMOD_Studio_Bank_GetLoadingState _FMODExt_func_FMOD_Studio_Bank_GetLoadingState
static int _FMODExt_func_FMOD_Studio_Bank_GetLoadingState(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    FMOD_STUDIO_LOADING_STATE state;
    ensure(ST, FMOD_Studio_Bank_GetLoadingState, FMOD_RESULT, FMOD_STUDIO_BANK*, FMOD_STUDIO_LOADING_STATE*);
    errCheck(FMOD_Studio_Bank_GetLoadingState(bank, &state));
    FMODExt_push_FMOD_STUDIO_LOADING_STATE(L, state);
    return 1;
}
#endif


/* FMOD_Studio_Bank_GetSampleLoadingState(input FMOD_STUDIO_BANK* bank, output FMOD_STUDIO_LOADING_STATE* state, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetSampleLoadingState
#define FMODExt_func_FMOD_Studio_Bank_GetSampleLoadingState _FMODExt_func_FMOD_Studio_Bank_GetSampleLoadingState
static int _FMODExt_func_FMOD_Studio_Bank_GetSampleLoadingState(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    FMOD_STUDIO_LOADING_STATE state;
    ensure(ST, FMOD_Studio_Bank_GetSampleLoadingState, FMOD_RESULT, FMOD_STUDIO_BANK*, FMOD_STUDIO_LOADING_STATE*);
    errCheck(FMOD_Studio_Bank_GetSampleLoadingState(bank, &state));
    FMODExt_push_FMOD_STUDIO_LOADING_STATE(L, state);
    return 1;
}
#endif


/* FMOD_Studio_Bank_GetStringCount(input FMOD_STUDIO_BANK* bank, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetStringCount
#define FMODExt_func_FMOD_Studio_Bank_GetStringCount _FMODExt_func_FMOD_Studio_Bank_GetStringCount
static int _FMODExt_func_FMOD_Studio_Bank_GetStringCount(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    int count;
    ensure(ST, FMOD_Studio_Bank_GetStringCount, FMOD_RESULT, FMOD_STUDIO_BANK*, int*);
    errCheck(FMOD_Studio_Bank_GetStringCount(bank, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_Bank_GetStringInfo(input FMOD_STUDIO_BANK* bank, input int index, output_ptr FMOD_GUID* id, unknown char* path, input int size, output int* retrieved, ) */


/* FMOD_Studio_Bank_GetEventCount(input FMOD_STUDIO_BANK* bank, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetEventCount
#define FMODExt_func_FMOD_Studio_Bank_GetEventCount _FMODExt_func_FMOD_Studio_Bank_GetEventCount
static int _FMODExt_func_FMOD_Studio_Bank_GetEventCount(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    int count;
    ensure(ST, FMOD_Studio_Bank_GetEventCount, FMOD_RESULT, FMOD_STUDIO_BANK*, int*);
    errCheck(FMOD_Studio_Bank_GetEventCount(bank, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_Bank_GetEventList(input FMOD_STUDIO_BANK* bank, output FMOD_STUDIO_EVENTDESCRIPTION** array, input int capacity, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetEventList
#define FMODExt_func_FMOD_Studio_Bank_GetEventList _FMODExt_func_FMOD_Studio_Bank_GetEventList
static int _FMODExt_func_FMOD_Studio_Bank_GetEventList(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    FMOD_STUDIO_EVENTDESCRIPTION* array;
    int capacity = FMODExt_check_int(L, 2);
    int count;
    ensure(ST, FMOD_Studio_Bank_GetEventList, FMOD_RESULT, FMOD_STUDIO_BANK*, FMOD_STUDIO_EVENTDESCRIPTION**, int, int*);
    errCheck(FMOD_Studio_Bank_GetEventList(bank, &array, capacity, &count));
    FMODExt_push_ptr_FMOD_STUDIO_EVENTDESCRIPTION(L, array);
    FMODExt_push_int(L, count);
    return 2;
}
#endif


/* FMOD_Studio_Bank_GetBusCount(input FMOD_STUDIO_BANK* bank, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetBusCount
#define FMODExt_func_FMOD_Studio_Bank_GetBusCount _FMODExt_func_FMOD_Studio_Bank_GetBusCount
static int _FMODExt_func_FMOD_Studio_Bank_GetBusCount(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    int count;
    ensure(ST, FMOD_Studio_Bank_GetBusCount, FMOD_RESULT, FMOD_STUDIO_BANK*, int*);
    errCheck(FMOD_Studio_Bank_GetBusCount(bank, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_Bank_GetBusList(input FMOD_STUDIO_BANK* bank, output FMOD_STUDIO_BUS** array, input int capacity, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetBusList
#define FMODExt_func_FMOD_Studio_Bank_GetBusList _FMODExt_func_FMOD_Studio_Bank_GetBusList
static int _FMODExt_func_FMOD_Studio_Bank_GetBusList(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    FMOD_STUDIO_BUS* array;
    int capacity = FMODExt_check_int(L, 2);
    int count;
    ensure(ST, FMOD_Studio_Bank_GetBusList, FMOD_RESULT, FMOD_STUDIO_BANK*, FMOD_STUDIO_BUS**, int, int*);
    errCheck(FMOD_Studio_Bank_GetBusList(bank, &array, capacity, &count));
    FMODExt_push_ptr_FMOD_STUDIO_BUS(L, array);
    FMODExt_push_int(L, count);
    return 2;
}
#endif


/* FMOD_Studio_Bank_GetVCACount(input FMOD_STUDIO_BANK* bank, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetVCACount
#define FMODExt_func_FMOD_Studio_Bank_GetVCACount _FMODExt_func_FMOD_Studio_Bank_GetVCACount
static int _FMODExt_func_FMOD_Studio_Bank_GetVCACount(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    int count;
    ensure(ST, FMOD_Studio_Bank_GetVCACount, FMOD_RESULT, FMOD_STUDIO_BANK*, int*);
    errCheck(FMOD_Studio_Bank_GetVCACount(bank, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_Bank_GetVCAList(input FMOD_STUDIO_BANK* bank, output FMOD_STUDIO_VCA** array, input int capacity, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_Bank_GetVCAList
#define FMODExt_func_FMOD_Studio_Bank_GetVCAList _FMODExt_func_FMOD_Studio_Bank_GetVCAList
static int _FMODExt_func_FMOD_Studio_Bank_GetVCAList(lua_State *L) {
    FMOD_STUDIO_BANK* bank = FMODExt_check_ptr_FMOD_STUDIO_BANK(L, 1);
    FMOD_STUDIO_VCA* array;
    int capacity = FMODExt_check_int(L, 2);
    int count;
    ensure(ST, FMOD_Studio_Bank_GetVCAList, FMOD_RESULT, FMOD_STUDIO_BANK*, FMOD_STUDIO_VCA**, int, int*);
    errCheck(FMOD_Studio_Bank_GetVCAList(bank, &array, capacity, &count));
    FMODExt_push_ptr_FMOD_STUDIO_VCA(L, array);
    FMODExt_push_int(L, count);
    return 2;
}
#endif


/* FMOD_Studio_Bank_GetUserData(input FMOD_STUDIO_BANK* bank, unknown void** userdata, ) */


/* FMOD_Studio_Bank_SetUserData(input FMOD_STUDIO_BANK* bank, unknown void* userdata, ) */


/* FMOD_Studio_CommandReplay_IsValid(input FMOD_STUDIO_COMMANDREPLAY* replay, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_IsValid
#define FMODExt_func_FMOD_Studio_CommandReplay_IsValid _FMODExt_func_FMOD_Studio_CommandReplay_IsValid
static int _FMODExt_func_FMOD_Studio_CommandReplay_IsValid(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    ensure(ST, FMOD_Studio_CommandReplay_IsValid, FMOD_BOOL, FMOD_STUDIO_COMMANDREPLAY*);
    FMOD_BOOL retval = FMOD_Studio_CommandReplay_IsValid(replay);
    lua_pushboolean(L, retval);
    return 1;
}
#endif


/* FMOD_Studio_CommandReplay_GetSystem(input FMOD_STUDIO_COMMANDREPLAY* replay, output FMOD_STUDIO_SYSTEM** system, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_GetSystem
#define FMODExt_func_FMOD_Studio_CommandReplay_GetSystem _FMODExt_func_FMOD_Studio_CommandReplay_GetSystem
static int _FMODExt_func_FMOD_Studio_CommandReplay_GetSystem(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    FMOD_STUDIO_SYSTEM* system;
    ensure(ST, FMOD_Studio_CommandReplay_GetSystem, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, FMOD_STUDIO_SYSTEM**);
    errCheck(FMOD_Studio_CommandReplay_GetSystem(replay, &system));
    FMODExt_push_ptr_FMOD_STUDIO_SYSTEM(L, system);
    return 1;
}
#endif


/* FMOD_Studio_CommandReplay_GetLength(input FMOD_STUDIO_COMMANDREPLAY* replay, output float* length, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_GetLength
#define FMODExt_func_FMOD_Studio_CommandReplay_GetLength _FMODExt_func_FMOD_Studio_CommandReplay_GetLength
static int _FMODExt_func_FMOD_Studio_CommandReplay_GetLength(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    float length;
    ensure(ST, FMOD_Studio_CommandReplay_GetLength, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, float*);
    errCheck(FMOD_Studio_CommandReplay_GetLength(replay, &length));
    FMODExt_push_float(L, length);
    return 1;
}
#endif


/* FMOD_Studio_CommandReplay_GetCommandCount(input FMOD_STUDIO_COMMANDREPLAY* replay, output int* count, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_GetCommandCount
#define FMODExt_func_FMOD_Studio_CommandReplay_GetCommandCount _FMODExt_func_FMOD_Studio_CommandReplay_GetCommandCount
static int _FMODExt_func_FMOD_Studio_CommandReplay_GetCommandCount(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    int count;
    ensure(ST, FMOD_Studio_CommandReplay_GetCommandCount, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, int*);
    errCheck(FMOD_Studio_CommandReplay_GetCommandCount(replay, &count));
    FMODExt_push_int(L, count);
    return 1;
}
#endif


/* FMOD_Studio_CommandReplay_GetCommandInfo(input FMOD_STUDIO_COMMANDREPLAY* replay, input int commandindex, output_ptr FMOD_STUDIO_COMMAND_INFO* info, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_GetCommandInfo
#define FMODExt_func_FMOD_Studio_CommandReplay_GetCommandInfo _FMODExt_func_FMOD_Studio_CommandReplay_GetCommandInfo
static int _FMODExt_func_FMOD_Studio_CommandReplay_GetCommandInfo(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    int commandindex = FMODExt_check_int(L, 2);
    FMOD_STUDIO_COMMAND_INFO* info = FMODExt_push_ptr_FMOD_STUDIO_COMMAND_INFO(L, NULL);
    ensure(ST, FMOD_Studio_CommandReplay_GetCommandInfo, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, int, FMOD_STUDIO_COMMAND_INFO*);
    errCheck(FMOD_Studio_CommandReplay_GetCommandInfo(replay, commandindex, info));
    lua_pushvalue(L, -1);
    return 1;
}
#endif


/* FMOD_Studio_CommandReplay_GetCommandString(input FMOD_STUDIO_COMMANDREPLAY* replay, input int commandindex, unknown char* buffer, input int length, ) */


/* FMOD_Studio_CommandReplay_GetCommandAtTime(input FMOD_STUDIO_COMMANDREPLAY* replay, input float time, output int* commandindex, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_GetCommandAtTime
#define FMODExt_func_FMOD_Studio_CommandReplay_GetCommandAtTime _FMODExt_func_FMOD_Studio_CommandReplay_GetCommandAtTime
static int _FMODExt_func_FMOD_Studio_CommandReplay_GetCommandAtTime(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    float time = FMODExt_check_float(L, 2);
    int commandindex;
    ensure(ST, FMOD_Studio_CommandReplay_GetCommandAtTime, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, float, int*);
    errCheck(FMOD_Studio_CommandReplay_GetCommandAtTime(replay, time, &commandindex));
    FMODExt_push_int(L, commandindex);
    return 1;
}
#endif


/* FMOD_Studio_CommandReplay_SetBankPath(input FMOD_STUDIO_COMMANDREPLAY* replay, input const char* bankPath, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_SetBankPath
#define FMODExt_func_FMOD_Studio_CommandReplay_SetBankPath _FMODExt_func_FMOD_Studio_CommandReplay_SetBankPath
static int _FMODExt_func_FMOD_Studio_CommandReplay_SetBankPath(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    const char* bankPath = FMODExt_check_ptr_char(L, 2);
    ensure(ST, FMOD_Studio_CommandReplay_SetBankPath, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, const char*);
    errCheck(FMOD_Studio_CommandReplay_SetBankPath(replay, bankPath));
    return 0;
}
#endif


/* FMOD_Studio_CommandReplay_Start(input FMOD_STUDIO_COMMANDREPLAY* replay, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_Start
#define FMODExt_func_FMOD_Studio_CommandReplay_Start _FMODExt_func_FMOD_Studio_CommandReplay_Start
static int _FMODExt_func_FMOD_Studio_CommandReplay_Start(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    ensure(ST, FMOD_Studio_CommandReplay_Start, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*);
    errCheck(FMOD_Studio_CommandReplay_Start(replay));
    return 0;
}
#endif


/* FMOD_Studio_CommandReplay_Stop(input FMOD_STUDIO_COMMANDREPLAY* replay, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_Stop
#define FMODExt_func_FMOD_Studio_CommandReplay_Stop _FMODExt_func_FMOD_Studio_CommandReplay_Stop
static int _FMODExt_func_FMOD_Studio_CommandReplay_Stop(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    ensure(ST, FMOD_Studio_CommandReplay_Stop, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*);
    errCheck(FMOD_Studio_CommandReplay_Stop(replay));
    return 0;
}
#endif


/* FMOD_Studio_CommandReplay_SeekToTime(input FMOD_STUDIO_COMMANDREPLAY* replay, input float time, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_SeekToTime
#define FMODExt_func_FMOD_Studio_CommandReplay_SeekToTime _FMODExt_func_FMOD_Studio_CommandReplay_SeekToTime
static int _FMODExt_func_FMOD_Studio_CommandReplay_SeekToTime(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    float time = FMODExt_check_float(L, 2);
    ensure(ST, FMOD_Studio_CommandReplay_SeekToTime, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, float);
    errCheck(FMOD_Studio_CommandReplay_SeekToTime(replay, time));
    return 0;
}
#endif


/* FMOD_Studio_CommandReplay_SeekToCommand(input FMOD_STUDIO_COMMANDREPLAY* replay, input int commandindex, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_SeekToCommand
#define FMODExt_func_FMOD_Studio_CommandReplay_SeekToCommand _FMODExt_func_FMOD_Studio_CommandReplay_SeekToCommand
static int _FMODExt_func_FMOD_Studio_CommandReplay_SeekToCommand(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    int commandindex = FMODExt_check_int(L, 2);
    ensure(ST, FMOD_Studio_CommandReplay_SeekToCommand, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, int);
    errCheck(FMOD_Studio_CommandReplay_SeekToCommand(replay, commandindex));
    return 0;
}
#endif


/* FMOD_Studio_CommandReplay_GetPaused(input FMOD_STUDIO_COMMANDREPLAY* replay, output FMOD_BOOL* paused, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_GetPaused
#define FMODExt_func_FMOD_Studio_CommandReplay_GetPaused _FMODExt_func_FMOD_Studio_CommandReplay_GetPaused
static int _FMODExt_func_FMOD_Studio_CommandReplay_GetPaused(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    FMOD_BOOL paused;
    ensure(ST, FMOD_Studio_CommandReplay_GetPaused, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, FMOD_BOOL*);
    errCheck(FMOD_Studio_CommandReplay_GetPaused(replay, &paused));
    FMODExt_push_FMOD_BOOL(L, paused);
    return 1;
}
#endif


/* FMOD_Studio_CommandReplay_SetPaused(input FMOD_STUDIO_COMMANDREPLAY* replay, input FMOD_BOOL paused, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_SetPaused
#define FMODExt_func_FMOD_Studio_CommandReplay_SetPaused _FMODExt_func_FMOD_Studio_CommandReplay_SetPaused
static int _FMODExt_func_FMOD_Studio_CommandReplay_SetPaused(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    FMOD_BOOL paused = FMODExt_check_FMOD_BOOL(L, 2);
    ensure(ST, FMOD_Studio_CommandReplay_SetPaused, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, FMOD_BOOL);
    errCheck(FMOD_Studio_CommandReplay_SetPaused(replay, paused));
    return 0;
}
#endif


/* FMOD_Studio_CommandReplay_GetPlaybackState(input FMOD_STUDIO_COMMANDREPLAY* replay, output FMOD_STUDIO_PLAYBACK_STATE* state, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_GetPlaybackState
#define FMODExt_func_FMOD_Studio_CommandReplay_GetPlaybackState _FMODExt_func_FMOD_Studio_CommandReplay_GetPlaybackState
static int _FMODExt_func_FMOD_Studio_CommandReplay_GetPlaybackState(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    FMOD_STUDIO_PLAYBACK_STATE state;
    ensure(ST, FMOD_Studio_CommandReplay_GetPlaybackState, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, FMOD_STUDIO_PLAYBACK_STATE*);
    errCheck(FMOD_Studio_CommandReplay_GetPlaybackState(replay, &state));
    FMODExt_push_FMOD_STUDIO_PLAYBACK_STATE(L, state);
    return 1;
}
#endif


/* FMOD_Studio_CommandReplay_GetCurrentCommand(input FMOD_STUDIO_COMMANDREPLAY* replay, output int* commandindex, output float* currenttime, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_GetCurrentCommand
#define FMODExt_func_FMOD_Studio_CommandReplay_GetCurrentCommand _FMODExt_func_FMOD_Studio_CommandReplay_GetCurrentCommand
static int _FMODExt_func_FMOD_Studio_CommandReplay_GetCurrentCommand(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    int commandindex;
    float currenttime;
    ensure(ST, FMOD_Studio_CommandReplay_GetCurrentCommand, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*, int*, float*);
    errCheck(FMOD_Studio_CommandReplay_GetCurrentCommand(replay, &commandindex, &currenttime));
    FMODExt_push_int(L, commandindex);
    FMODExt_push_float(L, currenttime);
    return 2;
}
#endif


/* FMOD_Studio_CommandReplay_Release(input FMOD_STUDIO_COMMANDREPLAY* replay, ) */
#ifndef FMODExt_func_FMOD_Studio_CommandReplay_Release
#define FMODExt_func_FMOD_Studio_CommandReplay_Release _FMODExt_func_FMOD_Studio_CommandReplay_Release
static int _FMODExt_func_FMOD_Studio_CommandReplay_Release(lua_State *L) {
    FMOD_STUDIO_COMMANDREPLAY* replay = FMODExt_check_ptr_FMOD_STUDIO_COMMANDREPLAY(L, 1);
    ensure(ST, FMOD_Studio_CommandReplay_Release, FMOD_RESULT, FMOD_STUDIO_COMMANDREPLAY*);
    errCheck(FMOD_Studio_CommandReplay_Release(replay));
    return 0;
}
#endif


/* FMOD_Studio_CommandReplay_SetFrameCallback(input FMOD_STUDIO_COMMANDREPLAY* replay, unknown FMOD_STUDIO_COMMANDREPLAY_FRAME_CALLBACK callback, ) */


/* FMOD_Studio_CommandReplay_SetLoadBankCallback(input FMOD_STUDIO_COMMANDREPLAY* replay, unknown FMOD_STUDIO_COMMANDREPLAY_LOAD_BANK_CALLBACK callback, ) */


/* FMOD_Studio_CommandReplay_SetCreateInstanceCallback(input FMOD_STUDIO_COMMANDREPLAY* replay, unknown FMOD_STUDIO_COMMANDREPLAY_CREATE_INSTANCE_CALLBACK callback, ) */


/* FMOD_Studio_CommandReplay_GetUserData(input FMOD_STUDIO_COMMANDREPLAY* replay, unknown void** userdata, ) */


/* FMOD_Studio_CommandReplay_SetUserData(input FMOD_STUDIO_COMMANDREPLAY* replay, unknown void* userdata, ) */



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

    #define addEnum(x) \
        lua_pushstring(L, #x); \
        lua_pushnumber(L, (lua_Number)CONCAT(FMOD_, x)); \
        lua_rawset(L, -4)

    addEnum(THREAD_TYPE_MIXER);
    addEnum(THREAD_TYPE_FEEDER);
    addEnum(THREAD_TYPE_STREAM);
    addEnum(THREAD_TYPE_FILE);
    addEnum(THREAD_TYPE_NONBLOCKING);
    addEnum(THREAD_TYPE_RECORD);
    addEnum(THREAD_TYPE_GEOMETRY);
    addEnum(THREAD_TYPE_PROFILER);
    addEnum(THREAD_TYPE_STUDIO_UPDATE);
    addEnum(THREAD_TYPE_STUDIO_LOAD_BANK);
    addEnum(THREAD_TYPE_STUDIO_LOAD_SAMPLE);
    addEnum(THREAD_TYPE_CONVOLUTION1);
    addEnum(THREAD_TYPE_CONVOLUTION2);
    addEnum(THREAD_TYPE_MAX);
    addEnum(OK);
    addEnum(ERR_BADCOMMAND);
    addEnum(ERR_CHANNEL_ALLOC);
    addEnum(ERR_CHANNEL_STOLEN);
    addEnum(ERR_DMA);
    addEnum(ERR_DSP_CONNECTION);
    addEnum(ERR_DSP_DONTPROCESS);
    addEnum(ERR_DSP_FORMAT);
    addEnum(ERR_DSP_INUSE);
    addEnum(ERR_DSP_NOTFOUND);
    addEnum(ERR_DSP_RESERVED);
    addEnum(ERR_DSP_SILENCE);
    addEnum(ERR_DSP_TYPE);
    addEnum(ERR_FILE_BAD);
    addEnum(ERR_FILE_COULDNOTSEEK);
    addEnum(ERR_FILE_DISKEJECTED);
    addEnum(ERR_FILE_EOF);
    addEnum(ERR_FILE_ENDOFDATA);
    addEnum(ERR_FILE_NOTFOUND);
    addEnum(ERR_FORMAT);
    addEnum(ERR_HEADER_MISMATCH);
    addEnum(ERR_HTTP);
    addEnum(ERR_HTTP_ACCESS);
    addEnum(ERR_HTTP_PROXY_AUTH);
    addEnum(ERR_HTTP_SERVER_ERROR);
    addEnum(ERR_HTTP_TIMEOUT);
    addEnum(ERR_INITIALIZATION);
    addEnum(ERR_INITIALIZED);
    addEnum(ERR_INTERNAL);
    addEnum(ERR_INVALID_FLOAT);
    addEnum(ERR_INVALID_HANDLE);
    addEnum(ERR_INVALID_PARAM);
    addEnum(ERR_INVALID_POSITION);
    addEnum(ERR_INVALID_SPEAKER);
    addEnum(ERR_INVALID_SYNCPOINT);
    addEnum(ERR_INVALID_THREAD);
    addEnum(ERR_INVALID_VECTOR);
    addEnum(ERR_MAXAUDIBLE);
    addEnum(ERR_MEMORY);
    addEnum(ERR_MEMORY_CANTPOINT);
    addEnum(ERR_NEEDS3D);
    addEnum(ERR_NEEDSHARDWARE);
    addEnum(ERR_NET_CONNECT);
    addEnum(ERR_NET_SOCKET_ERROR);
    addEnum(ERR_NET_URL);
    addEnum(ERR_NET_WOULD_BLOCK);
    addEnum(ERR_NOTREADY);
    addEnum(ERR_OUTPUT_ALLOCATED);
    addEnum(ERR_OUTPUT_CREATEBUFFER);
    addEnum(ERR_OUTPUT_DRIVERCALL);
    addEnum(ERR_OUTPUT_FORMAT);
    addEnum(ERR_OUTPUT_INIT);
    addEnum(ERR_OUTPUT_NODRIVERS);
    addEnum(ERR_PLUGIN);
    addEnum(ERR_PLUGIN_MISSING);
    addEnum(ERR_PLUGIN_RESOURCE);
    addEnum(ERR_PLUGIN_VERSION);
    addEnum(ERR_RECORD);
    addEnum(ERR_REVERB_CHANNELGROUP);
    addEnum(ERR_REVERB_INSTANCE);
    addEnum(ERR_SUBSOUNDS);
    addEnum(ERR_SUBSOUND_ALLOCATED);
    addEnum(ERR_SUBSOUND_CANTMOVE);
    addEnum(ERR_TAGNOTFOUND);
    addEnum(ERR_TOOMANYCHANNELS);
    addEnum(ERR_TRUNCATED);
    addEnum(ERR_UNIMPLEMENTED);
    addEnum(ERR_UNINITIALIZED);
    addEnum(ERR_UNSUPPORTED);
    addEnum(ERR_VERSION);
    addEnum(ERR_EVENT_ALREADY_LOADED);
    addEnum(ERR_EVENT_LIVEUPDATE_BUSY);
    addEnum(ERR_EVENT_LIVEUPDATE_MISMATCH);
    addEnum(ERR_EVENT_LIVEUPDATE_TIMEOUT);
    addEnum(ERR_EVENT_NOTFOUND);
    addEnum(ERR_STUDIO_UNINITIALIZED);
    addEnum(ERR_STUDIO_NOT_LOADED);
    addEnum(ERR_INVALID_STRING);
    addEnum(ERR_ALREADY_LOCKED);
    addEnum(ERR_NOT_LOCKED);
    addEnum(ERR_RECORD_DISCONNECTED);
    addEnum(ERR_TOOMANYSAMPLES);
    addEnum(CHANNELCONTROL_CHANNEL);
    addEnum(CHANNELCONTROL_CHANNELGROUP);
    addEnum(CHANNELCONTROL_MAX);
    addEnum(OUTPUTTYPE_AUTODETECT);
    addEnum(OUTPUTTYPE_UNKNOWN);
    addEnum(OUTPUTTYPE_NOSOUND);
    addEnum(OUTPUTTYPE_WAVWRITER);
    addEnum(OUTPUTTYPE_NOSOUND_NRT);
    addEnum(OUTPUTTYPE_WAVWRITER_NRT);
    addEnum(OUTPUTTYPE_WASAPI);
    addEnum(OUTPUTTYPE_ASIO);
    addEnum(OUTPUTTYPE_PULSEAUDIO);
    addEnum(OUTPUTTYPE_ALSA);
    addEnum(OUTPUTTYPE_COREAUDIO);
    addEnum(OUTPUTTYPE_AUDIOTRACK);
    addEnum(OUTPUTTYPE_OPENSL);
    addEnum(OUTPUTTYPE_AUDIOOUT);
    addEnum(OUTPUTTYPE_AUDIO3D);
    addEnum(OUTPUTTYPE_WEBAUDIO);
    addEnum(OUTPUTTYPE_NNAUDIO);
    addEnum(OUTPUTTYPE_WINSONIC);
    addEnum(OUTPUTTYPE_AAUDIO);
    addEnum(OUTPUTTYPE_AUDIOWORKLET);
    addEnum(OUTPUTTYPE_PHASE);
    addEnum(OUTPUTTYPE_OHAUDIO);
    addEnum(OUTPUTTYPE_MAX);
    addEnum(DEBUG_MODE_TTY);
    addEnum(DEBUG_MODE_FILE);
    addEnum(DEBUG_MODE_CALLBACK);
    addEnum(SPEAKERMODE_DEFAULT);
    addEnum(SPEAKERMODE_RAW);
    addEnum(SPEAKERMODE_MONO);
    addEnum(SPEAKERMODE_STEREO);
    addEnum(SPEAKERMODE_QUAD);
    addEnum(SPEAKERMODE_SURROUND);
    addEnum(SPEAKERMODE_5POINT1);
    addEnum(SPEAKERMODE_7POINT1);
    addEnum(SPEAKERMODE_7POINT1POINT4);
    addEnum(SPEAKERMODE_MAX);
    addEnum(SPEAKER_NONE);
    addEnum(SPEAKER_FRONT_LEFT);
    addEnum(SPEAKER_FRONT_RIGHT);
    addEnum(SPEAKER_FRONT_CENTER);
    addEnum(SPEAKER_LOW_FREQUENCY);
    addEnum(SPEAKER_SURROUND_LEFT);
    addEnum(SPEAKER_SURROUND_RIGHT);
    addEnum(SPEAKER_BACK_LEFT);
    addEnum(SPEAKER_BACK_RIGHT);
    addEnum(SPEAKER_TOP_FRONT_LEFT);
    addEnum(SPEAKER_TOP_FRONT_RIGHT);
    addEnum(SPEAKER_TOP_BACK_LEFT);
    addEnum(SPEAKER_TOP_BACK_RIGHT);
    addEnum(SPEAKER_MAX);
    addEnum(CHANNELORDER_DEFAULT);
    addEnum(CHANNELORDER_WAVEFORMAT);
    addEnum(CHANNELORDER_PROTOOLS);
    addEnum(CHANNELORDER_ALLMONO);
    addEnum(CHANNELORDER_ALLSTEREO);
    addEnum(CHANNELORDER_ALSA);
    addEnum(CHANNELORDER_MAX);
    addEnum(PLUGINTYPE_OUTPUT);
    addEnum(PLUGINTYPE_CODEC);
    addEnum(PLUGINTYPE_DSP);
    addEnum(PLUGINTYPE_MAX);
    addEnum(SOUND_TYPE_UNKNOWN);
    addEnum(SOUND_TYPE_AIFF);
    addEnum(SOUND_TYPE_ASF);
    addEnum(SOUND_TYPE_DLS);
    addEnum(SOUND_TYPE_FLAC);
    addEnum(SOUND_TYPE_FSB);
    addEnum(SOUND_TYPE_IT);
    addEnum(SOUND_TYPE_MIDI);
    addEnum(SOUND_TYPE_MOD);
    addEnum(SOUND_TYPE_MPEG);
    addEnum(SOUND_TYPE_OGGVORBIS);
    addEnum(SOUND_TYPE_PLAYLIST);
    addEnum(SOUND_TYPE_RAW);
    addEnum(SOUND_TYPE_S3M);
    addEnum(SOUND_TYPE_USER);
    addEnum(SOUND_TYPE_WAV);
    addEnum(SOUND_TYPE_XM);
    addEnum(SOUND_TYPE_XMA);
    addEnum(SOUND_TYPE_AUDIOQUEUE);
    addEnum(SOUND_TYPE_AT9);
    addEnum(SOUND_TYPE_VORBIS);
    addEnum(SOUND_TYPE_MEDIA_FOUNDATION);
    addEnum(SOUND_TYPE_MEDIACODEC);
    addEnum(SOUND_TYPE_FADPCM);
    addEnum(SOUND_TYPE_OPUS);
    addEnum(SOUND_TYPE_MAX);
    addEnum(SOUND_FORMAT_NONE);
    addEnum(SOUND_FORMAT_PCM8);
    addEnum(SOUND_FORMAT_PCM16);
    addEnum(SOUND_FORMAT_PCM24);
    addEnum(SOUND_FORMAT_PCM32);
    addEnum(SOUND_FORMAT_PCMFLOAT);
    addEnum(SOUND_FORMAT_BITSTREAM);
    addEnum(SOUND_FORMAT_MAX);
    addEnum(OPENSTATE_READY);
    addEnum(OPENSTATE_LOADING);
    addEnum(OPENSTATE_ERROR);
    addEnum(OPENSTATE_CONNECTING);
    addEnum(OPENSTATE_BUFFERING);
    addEnum(OPENSTATE_SEEKING);
    addEnum(OPENSTATE_PLAYING);
    addEnum(OPENSTATE_SETPOSITION);
    addEnum(OPENSTATE_MAX);
    addEnum(SOUNDGROUP_BEHAVIOR_FAIL);
    addEnum(SOUNDGROUP_BEHAVIOR_MUTE);
    addEnum(SOUNDGROUP_BEHAVIOR_STEALLOWEST);
    addEnum(SOUNDGROUP_BEHAVIOR_MAX);
    addEnum(CHANNELCONTROL_CALLBACK_END);
    addEnum(CHANNELCONTROL_CALLBACK_VIRTUALVOICE);
    addEnum(CHANNELCONTROL_CALLBACK_SYNCPOINT);
    addEnum(CHANNELCONTROL_CALLBACK_OCCLUSION);
    addEnum(CHANNELCONTROL_CALLBACK_MAX);
    addEnum(CHANNELCONTROL_DSP_HEAD);
    addEnum(CHANNELCONTROL_DSP_FADER);
    addEnum(CHANNELCONTROL_DSP_TAIL);
    addEnum(ERRORCALLBACK_INSTANCETYPE_NONE);
    addEnum(ERRORCALLBACK_INSTANCETYPE_SYSTEM);
    addEnum(ERRORCALLBACK_INSTANCETYPE_CHANNEL);
    addEnum(ERRORCALLBACK_INSTANCETYPE_CHANNELGROUP);
    addEnum(ERRORCALLBACK_INSTANCETYPE_CHANNELCONTROL);
    addEnum(ERRORCALLBACK_INSTANCETYPE_SOUND);
    addEnum(ERRORCALLBACK_INSTANCETYPE_SOUNDGROUP);
    addEnum(ERRORCALLBACK_INSTANCETYPE_DSP);
    addEnum(ERRORCALLBACK_INSTANCETYPE_DSPCONNECTION);
    addEnum(ERRORCALLBACK_INSTANCETYPE_GEOMETRY);
    addEnum(ERRORCALLBACK_INSTANCETYPE_REVERB3D);
    addEnum(ERRORCALLBACK_INSTANCETYPE_STUDIO_SYSTEM);
    addEnum(ERRORCALLBACK_INSTANCETYPE_STUDIO_EVENTDESCRIPTION);
    addEnum(ERRORCALLBACK_INSTANCETYPE_STUDIO_EVENTINSTANCE);
    addEnum(ERRORCALLBACK_INSTANCETYPE_STUDIO_PARAMETERINSTANCE);
    addEnum(ERRORCALLBACK_INSTANCETYPE_STUDIO_BUS);
    addEnum(ERRORCALLBACK_INSTANCETYPE_STUDIO_VCA);
    addEnum(ERRORCALLBACK_INSTANCETYPE_STUDIO_BANK);
    addEnum(ERRORCALLBACK_INSTANCETYPE_STUDIO_COMMANDREPLAY);
    addEnum(DSP_RESAMPLER_DEFAULT);
    addEnum(DSP_RESAMPLER_NOINTERP);
    addEnum(DSP_RESAMPLER_LINEAR);
    addEnum(DSP_RESAMPLER_CUBIC);
    addEnum(DSP_RESAMPLER_SPLINE);
    addEnum(DSP_RESAMPLER_MAX);
    addEnum(DSP_CALLBACK_DATAPARAMETERRELEASE);
    addEnum(DSP_CALLBACK_MAX);
    addEnum(DSPCONNECTION_TYPE_STANDARD);
    addEnum(DSPCONNECTION_TYPE_SIDECHAIN);
    addEnum(DSPCONNECTION_TYPE_SEND);
    addEnum(DSPCONNECTION_TYPE_SEND_SIDECHAIN);
    addEnum(DSPCONNECTION_TYPE_PREALLOCATED);
    addEnum(DSPCONNECTION_TYPE_MAX);
    addEnum(TAGTYPE_UNKNOWN);
    addEnum(TAGTYPE_ID3V1);
    addEnum(TAGTYPE_ID3V2);
    addEnum(TAGTYPE_VORBISCOMMENT);
    addEnum(TAGTYPE_SHOUTCAST);
    addEnum(TAGTYPE_ICECAST);
    addEnum(TAGTYPE_ASF);
    addEnum(TAGTYPE_MIDI);
    addEnum(TAGTYPE_PLAYLIST);
    addEnum(TAGTYPE_FMOD);
    addEnum(TAGTYPE_USER);
    addEnum(TAGTYPE_MAX);
    addEnum(TAGDATATYPE_BINARY);
    addEnum(TAGDATATYPE_INT);
    addEnum(TAGDATATYPE_FLOAT);
    addEnum(TAGDATATYPE_STRING);
    addEnum(TAGDATATYPE_STRING_UTF16);
    addEnum(TAGDATATYPE_STRING_UTF16BE);
    addEnum(TAGDATATYPE_STRING_UTF8);
    addEnum(TAGDATATYPE_MAX);
    addEnum(PORT_TYPE_MUSIC);
    addEnum(PORT_TYPE_COPYRIGHT_MUSIC);
    addEnum(PORT_TYPE_VOICE);
    addEnum(PORT_TYPE_CONTROLLER);
    addEnum(PORT_TYPE_PERSONAL);
    addEnum(PORT_TYPE_VIBRATION);
    addEnum(PORT_TYPE_AUX);
    addEnum(PORT_TYPE_PASSTHROUGH);
    addEnum(PORT_TYPE_VR_VIBRATION);
    addEnum(PORT_TYPE_MAX);
    addEnum(DSP_TYPE_UNKNOWN);
    addEnum(DSP_TYPE_MIXER);
    addEnum(DSP_TYPE_OSCILLATOR);
    addEnum(DSP_TYPE_LOWPASS);
    addEnum(DSP_TYPE_ITLOWPASS);
    addEnum(DSP_TYPE_HIGHPASS);
    addEnum(DSP_TYPE_ECHO);
    addEnum(DSP_TYPE_FADER);
    addEnum(DSP_TYPE_FLANGE);
    addEnum(DSP_TYPE_DISTORTION);
    addEnum(DSP_TYPE_NORMALIZE);
    addEnum(DSP_TYPE_LIMITER);
    addEnum(DSP_TYPE_PARAMEQ);
    addEnum(DSP_TYPE_PITCHSHIFT);
    addEnum(DSP_TYPE_CHORUS);
    addEnum(DSP_TYPE_ITECHO);
    addEnum(DSP_TYPE_COMPRESSOR);
    addEnum(DSP_TYPE_SFXREVERB);
    addEnum(DSP_TYPE_LOWPASS_SIMPLE);
    addEnum(DSP_TYPE_DELAY);
    addEnum(DSP_TYPE_TREMOLO);
    addEnum(DSP_TYPE_SEND);
    addEnum(DSP_TYPE_RETURN);
    addEnum(DSP_TYPE_HIGHPASS_SIMPLE);
    addEnum(DSP_TYPE_PAN);
    addEnum(DSP_TYPE_THREE_EQ);
    addEnum(DSP_TYPE_FFT);
    addEnum(DSP_TYPE_LOUDNESS_METER);
    addEnum(DSP_TYPE_CONVOLUTIONREVERB);
    addEnum(DSP_TYPE_CHANNELMIX);
    addEnum(DSP_TYPE_TRANSCEIVER);
    addEnum(DSP_TYPE_OBJECTPAN);
    addEnum(DSP_TYPE_MULTIBAND_EQ);
    addEnum(DSP_TYPE_MULTIBAND_DYNAMICS);
    addEnum(DSP_TYPE_MAX);
    addEnum(DSP_OSCILLATOR_TYPE);
    addEnum(DSP_OSCILLATOR_RATE);
    addEnum(DSP_LOWPASS_CUTOFF);
    addEnum(DSP_LOWPASS_RESONANCE);
    addEnum(DSP_ITLOWPASS_CUTOFF);
    addEnum(DSP_ITLOWPASS_RESONANCE);
    addEnum(DSP_HIGHPASS_CUTOFF);
    addEnum(DSP_HIGHPASS_RESONANCE);
    addEnum(DSP_ECHO_DELAY);
    addEnum(DSP_ECHO_FEEDBACK);
    addEnum(DSP_ECHO_DRYLEVEL);
    addEnum(DSP_ECHO_WETLEVEL);
    addEnum(DSP_ECHO_DELAYCHANGEMODE);
    addEnum(DSP_ECHO_DELAYCHANGEMODE_FADE);
    addEnum(DSP_ECHO_DELAYCHANGEMODE_LERP);
    addEnum(DSP_ECHO_DELAYCHANGEMODE_NONE);
    addEnum(DSP_FADER_GAIN);
    addEnum(DSP_FADER_OVERALL_GAIN);
    addEnum(DSP_FLANGE_MIX);
    addEnum(DSP_FLANGE_DEPTH);
    addEnum(DSP_FLANGE_RATE);
    addEnum(DSP_DISTORTION_LEVEL);
    addEnum(DSP_NORMALIZE_FADETIME);
    addEnum(DSP_NORMALIZE_THRESHOLD);
    addEnum(DSP_NORMALIZE_MAXAMP);
    addEnum(DSP_LIMITER_RELEASETIME);
    addEnum(DSP_LIMITER_CEILING);
    addEnum(DSP_LIMITER_MAXIMIZERGAIN);
    addEnum(DSP_LIMITER_MODE);
    addEnum(DSP_PARAMEQ_CENTER);
    addEnum(DSP_PARAMEQ_BANDWIDTH);
    addEnum(DSP_PARAMEQ_GAIN);
    addEnum(DSP_MULTIBAND_EQ_A_FILTER);
    addEnum(DSP_MULTIBAND_EQ_A_FREQUENCY);
    addEnum(DSP_MULTIBAND_EQ_A_Q);
    addEnum(DSP_MULTIBAND_EQ_A_GAIN);
    addEnum(DSP_MULTIBAND_EQ_B_FILTER);
    addEnum(DSP_MULTIBAND_EQ_B_FREQUENCY);
    addEnum(DSP_MULTIBAND_EQ_B_Q);
    addEnum(DSP_MULTIBAND_EQ_B_GAIN);
    addEnum(DSP_MULTIBAND_EQ_C_FILTER);
    addEnum(DSP_MULTIBAND_EQ_C_FREQUENCY);
    addEnum(DSP_MULTIBAND_EQ_C_Q);
    addEnum(DSP_MULTIBAND_EQ_C_GAIN);
    addEnum(DSP_MULTIBAND_EQ_D_FILTER);
    addEnum(DSP_MULTIBAND_EQ_D_FREQUENCY);
    addEnum(DSP_MULTIBAND_EQ_D_Q);
    addEnum(DSP_MULTIBAND_EQ_D_GAIN);
    addEnum(DSP_MULTIBAND_EQ_E_FILTER);
    addEnum(DSP_MULTIBAND_EQ_E_FREQUENCY);
    addEnum(DSP_MULTIBAND_EQ_E_Q);
    addEnum(DSP_MULTIBAND_EQ_E_GAIN);
    addEnum(DSP_MULTIBAND_EQ_FILTER_DISABLED);
    addEnum(DSP_MULTIBAND_EQ_FILTER_LOWPASS_12DB);
    addEnum(DSP_MULTIBAND_EQ_FILTER_LOWPASS_24DB);
    addEnum(DSP_MULTIBAND_EQ_FILTER_LOWPASS_48DB);
    addEnum(DSP_MULTIBAND_EQ_FILTER_HIGHPASS_12DB);
    addEnum(DSP_MULTIBAND_EQ_FILTER_HIGHPASS_24DB);
    addEnum(DSP_MULTIBAND_EQ_FILTER_HIGHPASS_48DB);
    addEnum(DSP_MULTIBAND_EQ_FILTER_LOWSHELF);
    addEnum(DSP_MULTIBAND_EQ_FILTER_HIGHSHELF);
    addEnum(DSP_MULTIBAND_EQ_FILTER_PEAKING);
    addEnum(DSP_MULTIBAND_EQ_FILTER_BANDPASS);
    addEnum(DSP_MULTIBAND_EQ_FILTER_NOTCH);
    addEnum(DSP_MULTIBAND_EQ_FILTER_ALLPASS);
    addEnum(DSP_MULTIBAND_EQ_FILTER_LOWPASS_6DB);
    addEnum(DSP_MULTIBAND_EQ_FILTER_HIGHPASS_6DB);
    addEnum(DSP_MULTIBAND_DYNAMICS_LOWER_FREQUENCY);
    addEnum(DSP_MULTIBAND_DYNAMICS_UPPER_FREQUENCY);
    addEnum(DSP_MULTIBAND_DYNAMICS_LINKED);
    addEnum(DSP_MULTIBAND_DYNAMICS_USE_SIDECHAIN);
    addEnum(DSP_MULTIBAND_DYNAMICS_A_MODE);
    addEnum(DSP_MULTIBAND_DYNAMICS_A_GAIN);
    addEnum(DSP_MULTIBAND_DYNAMICS_A_THRESHOLD);
    addEnum(DSP_MULTIBAND_DYNAMICS_A_RATIO);
    addEnum(DSP_MULTIBAND_DYNAMICS_A_ATTACK);
    addEnum(DSP_MULTIBAND_DYNAMICS_A_RELEASE);
    addEnum(DSP_MULTIBAND_DYNAMICS_A_GAIN_MAKEUP);
    addEnum(DSP_MULTIBAND_DYNAMICS_A_RESPONSE_DATA);
    addEnum(DSP_MULTIBAND_DYNAMICS_B_MODE);
    addEnum(DSP_MULTIBAND_DYNAMICS_B_GAIN);
    addEnum(DSP_MULTIBAND_DYNAMICS_B_THRESHOLD);
    addEnum(DSP_MULTIBAND_DYNAMICS_B_RATIO);
    addEnum(DSP_MULTIBAND_DYNAMICS_B_ATTACK);
    addEnum(DSP_MULTIBAND_DYNAMICS_B_RELEASE);
    addEnum(DSP_MULTIBAND_DYNAMICS_B_GAIN_MAKEUP);
    addEnum(DSP_MULTIBAND_DYNAMICS_B_RESPONSE_DATA);
    addEnum(DSP_MULTIBAND_DYNAMICS_C_MODE);
    addEnum(DSP_MULTIBAND_DYNAMICS_C_GAIN);
    addEnum(DSP_MULTIBAND_DYNAMICS_C_THRESHOLD);
    addEnum(DSP_MULTIBAND_DYNAMICS_C_RATIO);
    addEnum(DSP_MULTIBAND_DYNAMICS_C_ATTACK);
    addEnum(DSP_MULTIBAND_DYNAMICS_C_RELEASE);
    addEnum(DSP_MULTIBAND_DYNAMICS_C_GAIN_MAKEUP);
    addEnum(DSP_MULTIBAND_DYNAMICS_C_RESPONSE_DATA);
    addEnum(DSP_MULTIBAND_DYNAMICS_MODE_DISABLED);
    addEnum(DSP_MULTIBAND_DYNAMICS_MODE_COMPRESS_UP);
    addEnum(DSP_MULTIBAND_DYNAMICS_MODE_COMPRESS_DOWN);
    addEnum(DSP_MULTIBAND_DYNAMICS_MODE_EXPAND_UP);
    addEnum(DSP_MULTIBAND_DYNAMICS_MODE_EXPAND_DOWN);
    addEnum(DSP_PITCHSHIFT_PITCH);
    addEnum(DSP_PITCHSHIFT_FFTSIZE);
    addEnum(DSP_PITCHSHIFT_OVERLAP);
    addEnum(DSP_PITCHSHIFT_MAXCHANNELS);
    addEnum(DSP_CHORUS_MIX);
    addEnum(DSP_CHORUS_RATE);
    addEnum(DSP_CHORUS_DEPTH);
    addEnum(DSP_ITECHO_WETDRYMIX);
    addEnum(DSP_ITECHO_FEEDBACK);
    addEnum(DSP_ITECHO_LEFTDELAY);
    addEnum(DSP_ITECHO_RIGHTDELAY);
    addEnum(DSP_ITECHO_PANDELAY);
    addEnum(DSP_COMPRESSOR_THRESHOLD);
    addEnum(DSP_COMPRESSOR_RATIO);
    addEnum(DSP_COMPRESSOR_ATTACK);
    addEnum(DSP_COMPRESSOR_RELEASE);
    addEnum(DSP_COMPRESSOR_GAINMAKEUP);
    addEnum(DSP_COMPRESSOR_USESIDECHAIN);
    addEnum(DSP_COMPRESSOR_LINKED);
    addEnum(DSP_SFXREVERB_DECAYTIME);
    addEnum(DSP_SFXREVERB_EARLYDELAY);
    addEnum(DSP_SFXREVERB_LATEDELAY);
    addEnum(DSP_SFXREVERB_HFREFERENCE);
    addEnum(DSP_SFXREVERB_HFDECAYRATIO);
    addEnum(DSP_SFXREVERB_DIFFUSION);
    addEnum(DSP_SFXREVERB_DENSITY);
    addEnum(DSP_SFXREVERB_LOWSHELFFREQUENCY);
    addEnum(DSP_SFXREVERB_LOWSHELFGAIN);
    addEnum(DSP_SFXREVERB_HIGHCUT);
    addEnum(DSP_SFXREVERB_EARLYLATEMIX);
    addEnum(DSP_SFXREVERB_WETLEVEL);
    addEnum(DSP_SFXREVERB_DRYLEVEL);
    addEnum(DSP_LOWPASS_SIMPLE_CUTOFF);
    addEnum(DSP_DELAY_CH0);
    addEnum(DSP_DELAY_CH1);
    addEnum(DSP_DELAY_CH2);
    addEnum(DSP_DELAY_CH3);
    addEnum(DSP_DELAY_CH4);
    addEnum(DSP_DELAY_CH5);
    addEnum(DSP_DELAY_CH6);
    addEnum(DSP_DELAY_CH7);
    addEnum(DSP_DELAY_CH8);
    addEnum(DSP_DELAY_CH9);
    addEnum(DSP_DELAY_CH10);
    addEnum(DSP_DELAY_CH11);
    addEnum(DSP_DELAY_CH12);
    addEnum(DSP_DELAY_CH13);
    addEnum(DSP_DELAY_CH14);
    addEnum(DSP_DELAY_CH15);
    addEnum(DSP_DELAY_MAXDELAY);
    addEnum(DSP_TREMOLO_FREQUENCY);
    addEnum(DSP_TREMOLO_DEPTH);
    addEnum(DSP_TREMOLO_SHAPE);
    addEnum(DSP_TREMOLO_SKEW);
    addEnum(DSP_TREMOLO_DUTY);
    addEnum(DSP_TREMOLO_SQUARE);
    addEnum(DSP_TREMOLO_PHASE);
    addEnum(DSP_TREMOLO_SPREAD);
    addEnum(DSP_SEND_RETURNID);
    addEnum(DSP_SEND_LEVEL);
    addEnum(DSP_RETURN_ID);
    addEnum(DSP_RETURN_INPUT_SPEAKER_MODE);
    addEnum(DSP_HIGHPASS_SIMPLE_CUTOFF);
    addEnum(DSP_PAN_2D_STEREO_MODE_DISTRIBUTED);
    addEnum(DSP_PAN_2D_STEREO_MODE_DISCRETE);
    addEnum(DSP_PAN_MODE_MONO);
    addEnum(DSP_PAN_MODE_STEREO);
    addEnum(DSP_PAN_MODE_SURROUND);
    addEnum(DSP_PAN_3D_ROLLOFF_LINEARSQUARED);
    addEnum(DSP_PAN_3D_ROLLOFF_LINEAR);
    addEnum(DSP_PAN_3D_ROLLOFF_INVERSE);
    addEnum(DSP_PAN_3D_ROLLOFF_INVERSETAPERED);
    addEnum(DSP_PAN_3D_ROLLOFF_CUSTOM);
    addEnum(DSP_PAN_3D_EXTENT_MODE_AUTO);
    addEnum(DSP_PAN_3D_EXTENT_MODE_USER);
    addEnum(DSP_PAN_3D_EXTENT_MODE_OFF);
    addEnum(DSP_PAN_MODE);
    addEnum(DSP_PAN_2D_STEREO_POSITION);
    addEnum(DSP_PAN_2D_DIRECTION);
    addEnum(DSP_PAN_2D_EXTENT);
    addEnum(DSP_PAN_2D_ROTATION);
    addEnum(DSP_PAN_2D_LFE_LEVEL);
    addEnum(DSP_PAN_2D_STEREO_MODE);
    addEnum(DSP_PAN_2D_STEREO_SEPARATION);
    addEnum(DSP_PAN_2D_STEREO_AXIS);
    addEnum(DSP_PAN_ENABLED_SPEAKERS);
    addEnum(DSP_PAN_3D_POSITION);
    addEnum(DSP_PAN_3D_ROLLOFF);
    addEnum(DSP_PAN_3D_MIN_DISTANCE);
    addEnum(DSP_PAN_3D_MAX_DISTANCE);
    addEnum(DSP_PAN_3D_EXTENT_MODE);
    addEnum(DSP_PAN_3D_SOUND_SIZE);
    addEnum(DSP_PAN_3D_MIN_EXTENT);
    addEnum(DSP_PAN_3D_PAN_BLEND);
    addEnum(DSP_PAN_LFE_UPMIX_ENABLED);
    addEnum(DSP_PAN_OVERALL_GAIN);
    addEnum(DSP_PAN_SURROUND_SPEAKER_MODE);
    addEnum(DSP_PAN_2D_HEIGHT_BLEND);
    addEnum(DSP_PAN_ATTENUATION_RANGE);
    addEnum(DSP_PAN_OVERRIDE_RANGE);
    addEnum(DSP_THREE_EQ_CROSSOVERSLOPE_12DB);
    addEnum(DSP_THREE_EQ_CROSSOVERSLOPE_24DB);
    addEnum(DSP_THREE_EQ_CROSSOVERSLOPE_48DB);
    addEnum(DSP_THREE_EQ_LOWGAIN);
    addEnum(DSP_THREE_EQ_MIDGAIN);
    addEnum(DSP_THREE_EQ_HIGHGAIN);
    addEnum(DSP_THREE_EQ_LOWCROSSOVER);
    addEnum(DSP_THREE_EQ_HIGHCROSSOVER);
    addEnum(DSP_THREE_EQ_CROSSOVERSLOPE);
    addEnum(DSP_FFT_WINDOW_RECT);
    addEnum(DSP_FFT_WINDOW_TRIANGLE);
    addEnum(DSP_FFT_WINDOW_HAMMING);
    addEnum(DSP_FFT_WINDOW_HANNING);
    addEnum(DSP_FFT_WINDOW_BLACKMAN);
    addEnum(DSP_FFT_WINDOW_BLACKMANHARRIS);
    addEnum(DSP_FFT_DOWNMIX_NONE);
    addEnum(DSP_FFT_DOWNMIX_MONO);
    addEnum(DSP_FFT_WINDOWSIZE);
    addEnum(DSP_FFT_WINDOW);
    addEnum(DSP_FFT_BAND_START_FREQ);
    addEnum(DSP_FFT_BAND_STOP_FREQ);
    addEnum(DSP_FFT_SPECTRUMDATA);
    addEnum(DSP_FFT_RMS);
    addEnum(DSP_FFT_SPECTRAL_CENTROID);
    addEnum(DSP_FFT_IMMEDIATE_MODE);
    addEnum(DSP_FFT_DOWNMIX);
    addEnum(DSP_FFT_CHANNEL);
    addEnum(DSP_LOUDNESS_METER_STATE);
    addEnum(DSP_LOUDNESS_METER_WEIGHTING);
    addEnum(DSP_LOUDNESS_METER_INFO);
    addEnum(DSP_LOUDNESS_METER_STATE_RESET_INTEGRATED);
    addEnum(DSP_LOUDNESS_METER_STATE_RESET_MAXPEAK);
    addEnum(DSP_LOUDNESS_METER_STATE_RESET_ALL);
    addEnum(DSP_LOUDNESS_METER_STATE_PAUSED);
    addEnum(DSP_LOUDNESS_METER_STATE_ANALYZING);
    addEnum(DSP_CONVOLUTION_REVERB_PARAM_IR);
    addEnum(DSP_CONVOLUTION_REVERB_PARAM_WET);
    addEnum(DSP_CONVOLUTION_REVERB_PARAM_DRY);
    addEnum(DSP_CONVOLUTION_REVERB_PARAM_LINKED);
    addEnum(DSP_CHANNELMIX_OUTPUT_DEFAULT);
    addEnum(DSP_CHANNELMIX_OUTPUT_ALLMONO);
    addEnum(DSP_CHANNELMIX_OUTPUT_ALLSTEREO);
    addEnum(DSP_CHANNELMIX_OUTPUT_ALLQUAD);
    addEnum(DSP_CHANNELMIX_OUTPUT_ALL5POINT1);
    addEnum(DSP_CHANNELMIX_OUTPUT_ALL7POINT1);
    addEnum(DSP_CHANNELMIX_OUTPUT_ALLLFE);
    addEnum(DSP_CHANNELMIX_OUTPUT_ALL7POINT1POINT4);
    addEnum(DSP_CHANNELMIX_OUTPUTGROUPING);
    addEnum(DSP_CHANNELMIX_GAIN_CH0);
    addEnum(DSP_CHANNELMIX_GAIN_CH1);
    addEnum(DSP_CHANNELMIX_GAIN_CH2);
    addEnum(DSP_CHANNELMIX_GAIN_CH3);
    addEnum(DSP_CHANNELMIX_GAIN_CH4);
    addEnum(DSP_CHANNELMIX_GAIN_CH5);
    addEnum(DSP_CHANNELMIX_GAIN_CH6);
    addEnum(DSP_CHANNELMIX_GAIN_CH7);
    addEnum(DSP_CHANNELMIX_GAIN_CH8);
    addEnum(DSP_CHANNELMIX_GAIN_CH9);
    addEnum(DSP_CHANNELMIX_GAIN_CH10);
    addEnum(DSP_CHANNELMIX_GAIN_CH11);
    addEnum(DSP_CHANNELMIX_GAIN_CH12);
    addEnum(DSP_CHANNELMIX_GAIN_CH13);
    addEnum(DSP_CHANNELMIX_GAIN_CH14);
    addEnum(DSP_CHANNELMIX_GAIN_CH15);
    addEnum(DSP_CHANNELMIX_GAIN_CH16);
    addEnum(DSP_CHANNELMIX_GAIN_CH17);
    addEnum(DSP_CHANNELMIX_GAIN_CH18);
    addEnum(DSP_CHANNELMIX_GAIN_CH19);
    addEnum(DSP_CHANNELMIX_GAIN_CH20);
    addEnum(DSP_CHANNELMIX_GAIN_CH21);
    addEnum(DSP_CHANNELMIX_GAIN_CH22);
    addEnum(DSP_CHANNELMIX_GAIN_CH23);
    addEnum(DSP_CHANNELMIX_GAIN_CH24);
    addEnum(DSP_CHANNELMIX_GAIN_CH25);
    addEnum(DSP_CHANNELMIX_GAIN_CH26);
    addEnum(DSP_CHANNELMIX_GAIN_CH27);
    addEnum(DSP_CHANNELMIX_GAIN_CH28);
    addEnum(DSP_CHANNELMIX_GAIN_CH29);
    addEnum(DSP_CHANNELMIX_GAIN_CH30);
    addEnum(DSP_CHANNELMIX_GAIN_CH31);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH0);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH1);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH2);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH3);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH4);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH5);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH6);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH7);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH8);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH9);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH10);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH11);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH12);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH13);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH14);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH15);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH16);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH17);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH18);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH19);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH20);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH21);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH22);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH23);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH24);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH25);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH26);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH27);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH28);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH29);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH30);
    addEnum(DSP_CHANNELMIX_OUTPUT_CH31);
    addEnum(DSP_TRANSCEIVER_SPEAKERMODE_AUTO);
    addEnum(DSP_TRANSCEIVER_SPEAKERMODE_MONO);
    addEnum(DSP_TRANSCEIVER_SPEAKERMODE_STEREO);
    addEnum(DSP_TRANSCEIVER_SPEAKERMODE_SURROUND);
    addEnum(DSP_TRANSCEIVER_TRANSMIT);
    addEnum(DSP_TRANSCEIVER_GAIN);
    addEnum(DSP_TRANSCEIVER_CHANNEL);
    addEnum(DSP_TRANSCEIVER_TRANSMITSPEAKERMODE);
    addEnum(DSP_OBJECTPAN_3D_POSITION);
    addEnum(DSP_OBJECTPAN_3D_ROLLOFF);
    addEnum(DSP_OBJECTPAN_3D_MIN_DISTANCE);
    addEnum(DSP_OBJECTPAN_3D_MAX_DISTANCE);
    addEnum(DSP_OBJECTPAN_3D_EXTENT_MODE);
    addEnum(DSP_OBJECTPAN_3D_SOUND_SIZE);
    addEnum(DSP_OBJECTPAN_3D_MIN_EXTENT);
    addEnum(DSP_OBJECTPAN_OVERALL_GAIN);
    addEnum(DSP_OBJECTPAN_OUTPUTGAIN);
    addEnum(DSP_OBJECTPAN_ATTENUATION_RANGE);
    addEnum(DSP_OBJECTPAN_OVERRIDE_RANGE);
    addEnum(DSP_PROCESS_PERFORM);
    addEnum(DSP_PROCESS_QUERY);
    addEnum(DSP_PAN_SURROUND_DEFAULT);
    addEnum(DSP_PAN_SURROUND_ROTATION_NOT_BIASED);
    addEnum(DSP_PARAMETER_TYPE_FLOAT);
    addEnum(DSP_PARAMETER_TYPE_INT);
    addEnum(DSP_PARAMETER_TYPE_BOOL);
    addEnum(DSP_PARAMETER_TYPE_DATA);
    addEnum(DSP_PARAMETER_TYPE_MAX);
    addEnum(DSP_PARAMETER_FLOAT_MAPPING_TYPE_LINEAR);
    addEnum(DSP_PARAMETER_FLOAT_MAPPING_TYPE_AUTO);
    addEnum(DSP_PARAMETER_FLOAT_MAPPING_TYPE_PIECEWISE_LINEAR);
    addEnum(DSP_PARAMETER_DATA_TYPE_USER);
    addEnum(DSP_PARAMETER_DATA_TYPE_OVERALLGAIN);
    addEnum(DSP_PARAMETER_DATA_TYPE_3DATTRIBUTES);
    addEnum(DSP_PARAMETER_DATA_TYPE_SIDECHAIN);
    addEnum(DSP_PARAMETER_DATA_TYPE_FFT);
    addEnum(DSP_PARAMETER_DATA_TYPE_3DATTRIBUTES_MULTI);
    addEnum(DSP_PARAMETER_DATA_TYPE_ATTENUATION_RANGE);
    addEnum(DSP_PARAMETER_DATA_TYPE_DYNAMIC_RESPONSE);
    addEnum(STUDIO_LOADING_STATE_UNLOADING);
    addEnum(STUDIO_LOADING_STATE_UNLOADED);
    addEnum(STUDIO_LOADING_STATE_LOADING);
    addEnum(STUDIO_LOADING_STATE_LOADED);
    addEnum(STUDIO_LOADING_STATE_ERROR);
    addEnum(STUDIO_LOAD_MEMORY);
    addEnum(STUDIO_LOAD_MEMORY_POINT);
    addEnum(STUDIO_PARAMETER_GAME_CONTROLLED);
    addEnum(STUDIO_PARAMETER_AUTOMATIC_DISTANCE);
    addEnum(STUDIO_PARAMETER_AUTOMATIC_EVENT_CONE_ANGLE);
    addEnum(STUDIO_PARAMETER_AUTOMATIC_EVENT_ORIENTATION);
    addEnum(STUDIO_PARAMETER_AUTOMATIC_DIRECTION);
    addEnum(STUDIO_PARAMETER_AUTOMATIC_ELEVATION);
    addEnum(STUDIO_PARAMETER_AUTOMATIC_LISTENER_ORIENTATION);
    addEnum(STUDIO_PARAMETER_AUTOMATIC_SPEED);
    addEnum(STUDIO_PARAMETER_AUTOMATIC_SPEED_ABSOLUTE);
    addEnum(STUDIO_PARAMETER_AUTOMATIC_DISTANCE_NORMALIZED);
    addEnum(STUDIO_PARAMETER_MAX);
    addEnum(STUDIO_USER_PROPERTY_TYPE_INTEGER);
    addEnum(STUDIO_USER_PROPERTY_TYPE_BOOLEAN);
    addEnum(STUDIO_USER_PROPERTY_TYPE_FLOAT);
    addEnum(STUDIO_USER_PROPERTY_TYPE_STRING);
    addEnum(STUDIO_EVENT_PROPERTY_CHANNELPRIORITY);
    addEnum(STUDIO_EVENT_PROPERTY_SCHEDULE_DELAY);
    addEnum(STUDIO_EVENT_PROPERTY_SCHEDULE_LOOKAHEAD);
    addEnum(STUDIO_EVENT_PROPERTY_MINIMUM_DISTANCE);
    addEnum(STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE);
    addEnum(STUDIO_EVENT_PROPERTY_COOLDOWN);
    addEnum(STUDIO_EVENT_PROPERTY_MAX);
    addEnum(STUDIO_PLAYBACK_PLAYING);
    addEnum(STUDIO_PLAYBACK_SUSTAINING);
    addEnum(STUDIO_PLAYBACK_STOPPED);
    addEnum(STUDIO_PLAYBACK_STARTING);
    addEnum(STUDIO_PLAYBACK_STOPPING);
    addEnum(STUDIO_STOP_ALLOWFADEOUT);
    addEnum(STUDIO_STOP_IMMEDIATE);
    addEnum(STUDIO_INSTANCETYPE_NONE);
    addEnum(STUDIO_INSTANCETYPE_SYSTEM);
    addEnum(STUDIO_INSTANCETYPE_EVENTDESCRIPTION);
    addEnum(STUDIO_INSTANCETYPE_EVENTINSTANCE);
    addEnum(STUDIO_INSTANCETYPE_PARAMETERINSTANCE);
    addEnum(STUDIO_INSTANCETYPE_BUS);
    addEnum(STUDIO_INSTANCETYPE_VCA);
    addEnum(STUDIO_INSTANCETYPE_BANK);
    addEnum(STUDIO_INSTANCETYPE_COMMANDREPLAY);
    addEnum(VERSION);
    addEnum(BUILDNUMBER);
    addEnum(DEBUG_LEVEL_NONE);
    addEnum(DEBUG_LEVEL_ERROR);
    addEnum(DEBUG_LEVEL_WARNING);
    addEnum(DEBUG_LEVEL_LOG);
    addEnum(DEBUG_TYPE_MEMORY);
    addEnum(DEBUG_TYPE_FILE);
    addEnum(DEBUG_TYPE_CODEC);
    addEnum(DEBUG_TYPE_TRACE);
    addEnum(DEBUG_TYPE_VIRTUAL);
    addEnum(DEBUG_DISPLAY_TIMESTAMPS);
    addEnum(DEBUG_DISPLAY_LINENUMBERS);
    addEnum(DEBUG_DISPLAY_THREAD);
    addEnum(MEMORY_NORMAL);
    addEnum(MEMORY_STREAM_FILE);
    addEnum(MEMORY_STREAM_DECODE);
    addEnum(MEMORY_SAMPLEDATA);
    addEnum(MEMORY_DSP_BUFFER);
    addEnum(MEMORY_PLUGIN);
    addEnum(MEMORY_PERSISTENT);
    addEnum(MEMORY_ALL);
    addEnum(INIT_NORMAL);
    addEnum(INIT_STREAM_FROM_UPDATE);
    addEnum(INIT_MIX_FROM_UPDATE);
    addEnum(INIT_3D_RIGHTHANDED);
    addEnum(INIT_CLIP_OUTPUT);
    addEnum(INIT_CHANNEL_LOWPASS);
    addEnum(INIT_CHANNEL_DISTANCEFILTER);
    addEnum(INIT_PROFILE_ENABLE);
    addEnum(INIT_VOL0_BECOMES_VIRTUAL);
    addEnum(INIT_GEOMETRY_USECLOSEST);
    addEnum(INIT_PREFER_DOLBY_DOWNMIX);
    addEnum(INIT_THREAD_UNSAFE);
    addEnum(INIT_PROFILE_METER_ALL);
    addEnum(INIT_MEMORY_TRACKING);
    addEnum(DRIVER_STATE_CONNECTED);
    addEnum(DRIVER_STATE_DEFAULT);
    addEnum(TIMEUNIT_MS);
    addEnum(TIMEUNIT_PCM);
    addEnum(TIMEUNIT_PCMBYTES);
    addEnum(TIMEUNIT_RAWBYTES);
    addEnum(TIMEUNIT_PCMFRACTION);
    addEnum(TIMEUNIT_MODORDER);
    addEnum(TIMEUNIT_MODROW);
    addEnum(TIMEUNIT_MODPATTERN);
    addEnum(SYSTEM_CALLBACK_DEVICELISTCHANGED);
    addEnum(SYSTEM_CALLBACK_DEVICELOST);
    addEnum(SYSTEM_CALLBACK_MEMORYALLOCATIONFAILED);
    addEnum(SYSTEM_CALLBACK_THREADCREATED);
    addEnum(SYSTEM_CALLBACK_BADDSPCONNECTION);
    addEnum(SYSTEM_CALLBACK_PREMIX);
    addEnum(SYSTEM_CALLBACK_POSTMIX);
    addEnum(SYSTEM_CALLBACK_ERROR);
    addEnum(SYSTEM_CALLBACK_THREADDESTROYED);
    addEnum(SYSTEM_CALLBACK_PREUPDATE);
    addEnum(SYSTEM_CALLBACK_POSTUPDATE);
    addEnum(SYSTEM_CALLBACK_RECORDLISTCHANGED);
    addEnum(SYSTEM_CALLBACK_BUFFEREDNOMIX);
    addEnum(SYSTEM_CALLBACK_DEVICEREINITIALIZE);
    addEnum(SYSTEM_CALLBACK_OUTPUTUNDERRUN);
    addEnum(SYSTEM_CALLBACK_RECORDPOSITIONCHANGED);
    addEnum(SYSTEM_CALLBACK_ALL);
    addEnum(DEFAULT);
    addEnum(LOOP_OFF);
    addEnum(LOOP_NORMAL);
    addEnum(LOOP_BIDI);
    addEnum(2D);
    addEnum(3D);
    addEnum(CREATESTREAM);
    addEnum(CREATESAMPLE);
    addEnum(CREATECOMPRESSEDSAMPLE);
    addEnum(OPENUSER);
    addEnum(OPENMEMORY);
    addEnum(OPENMEMORY_POINT);
    addEnum(OPENRAW);
    addEnum(OPENONLY);
    addEnum(ACCURATETIME);
    addEnum(MPEGSEARCH);
    addEnum(NONBLOCKING);
    addEnum(UNIQUE);
    addEnum(3D_HEADRELATIVE);
    addEnum(3D_WORLDRELATIVE);
    addEnum(3D_INVERSEROLLOFF);
    addEnum(3D_LINEARROLLOFF);
    addEnum(3D_LINEARSQUAREROLLOFF);
    addEnum(3D_INVERSETAPEREDROLLOFF);
    addEnum(3D_CUSTOMROLLOFF);
    addEnum(3D_IGNOREGEOMETRY);
    addEnum(IGNORETAGS);
    addEnum(LOWMEM);
    addEnum(VIRTUAL_PLAYFROMSTART);
    addEnum(CHANNELMASK_FRONT_LEFT);
    addEnum(CHANNELMASK_FRONT_RIGHT);
    addEnum(CHANNELMASK_FRONT_CENTER);
    addEnum(CHANNELMASK_LOW_FREQUENCY);
    addEnum(CHANNELMASK_SURROUND_LEFT);
    addEnum(CHANNELMASK_SURROUND_RIGHT);
    addEnum(CHANNELMASK_BACK_LEFT);
    addEnum(CHANNELMASK_BACK_RIGHT);
    addEnum(CHANNELMASK_BACK_CENTER);
    addEnum(CHANNELMASK_MONO);
    addEnum(CHANNELMASK_STEREO);
    addEnum(CHANNELMASK_LRC);
    addEnum(CHANNELMASK_QUAD);
    addEnum(CHANNELMASK_SURROUND);
    addEnum(CHANNELMASK_5POINT1);
    addEnum(CHANNELMASK_5POINT1_REARS);
    addEnum(CHANNELMASK_7POINT0);
    addEnum(CHANNELMASK_7POINT1);
    addEnum(PORT_INDEX_NONE);
    addEnum(THREAD_PRIORITY_PLATFORM_MIN);
    addEnum(THREAD_PRIORITY_PLATFORM_MAX);
    addEnum(THREAD_PRIORITY_DEFAULT);
    addEnum(THREAD_PRIORITY_LOW);
    addEnum(THREAD_PRIORITY_MEDIUM);
    addEnum(THREAD_PRIORITY_HIGH);
    addEnum(THREAD_PRIORITY_VERY_HIGH);
    addEnum(THREAD_PRIORITY_EXTREME);
    addEnum(THREAD_PRIORITY_CRITICAL);
    addEnum(THREAD_PRIORITY_MIXER);
    addEnum(THREAD_PRIORITY_FEEDER);
    addEnum(THREAD_PRIORITY_STREAM);
    addEnum(THREAD_PRIORITY_FILE);
    addEnum(THREAD_PRIORITY_NONBLOCKING);
    addEnum(THREAD_PRIORITY_RECORD);
    addEnum(THREAD_PRIORITY_GEOMETRY);
    addEnum(THREAD_PRIORITY_PROFILER);
    addEnum(THREAD_PRIORITY_STUDIO_UPDATE);
    addEnum(THREAD_PRIORITY_STUDIO_LOAD_BANK);
    addEnum(THREAD_PRIORITY_STUDIO_LOAD_SAMPLE);
    addEnum(THREAD_PRIORITY_CONVOLUTION1);
    addEnum(THREAD_PRIORITY_CONVOLUTION2);
    addEnum(THREAD_STACK_SIZE_DEFAULT);
    addEnum(THREAD_STACK_SIZE_MIXER);
    addEnum(THREAD_STACK_SIZE_FEEDER);
    addEnum(THREAD_STACK_SIZE_STREAM);
    addEnum(THREAD_STACK_SIZE_FILE);
    addEnum(THREAD_STACK_SIZE_NONBLOCKING);
    addEnum(THREAD_STACK_SIZE_RECORD);
    addEnum(THREAD_STACK_SIZE_GEOMETRY);
    addEnum(THREAD_STACK_SIZE_PROFILER);
    addEnum(THREAD_STACK_SIZE_STUDIO_UPDATE);
    addEnum(THREAD_STACK_SIZE_STUDIO_LOAD_BANK);
    addEnum(THREAD_STACK_SIZE_STUDIO_LOAD_SAMPLE);
    addEnum(THREAD_STACK_SIZE_CONVOLUTION1);
    addEnum(THREAD_STACK_SIZE_CONVOLUTION2);
    addEnum(THREAD_AFFINITY_GROUP_DEFAULT);
    addEnum(THREAD_AFFINITY_GROUP_A);
    addEnum(THREAD_AFFINITY_GROUP_B);
    addEnum(THREAD_AFFINITY_GROUP_C);
    addEnum(THREAD_AFFINITY_MIXER);
    addEnum(THREAD_AFFINITY_FEEDER);
    addEnum(THREAD_AFFINITY_STREAM);
    addEnum(THREAD_AFFINITY_FILE);
    addEnum(THREAD_AFFINITY_NONBLOCKING);
    addEnum(THREAD_AFFINITY_RECORD);
    addEnum(THREAD_AFFINITY_GEOMETRY);
    addEnum(THREAD_AFFINITY_PROFILER);
    addEnum(THREAD_AFFINITY_STUDIO_UPDATE);
    addEnum(THREAD_AFFINITY_STUDIO_LOAD_BANK);
    addEnum(THREAD_AFFINITY_STUDIO_LOAD_SAMPLE);
    addEnum(THREAD_AFFINITY_CONVOLUTION1);
    addEnum(THREAD_AFFINITY_CONVOLUTION2);
    addEnum(THREAD_AFFINITY_CORE_ALL);
    addEnum(THREAD_AFFINITY_CORE_0);
    addEnum(THREAD_AFFINITY_CORE_1);
    addEnum(THREAD_AFFINITY_CORE_2);
    addEnum(THREAD_AFFINITY_CORE_3);
    addEnum(THREAD_AFFINITY_CORE_4);
    addEnum(THREAD_AFFINITY_CORE_5);
    addEnum(THREAD_AFFINITY_CORE_6);
    addEnum(THREAD_AFFINITY_CORE_7);
    addEnum(THREAD_AFFINITY_CORE_8);
    addEnum(THREAD_AFFINITY_CORE_9);
    addEnum(THREAD_AFFINITY_CORE_10);
    addEnum(THREAD_AFFINITY_CORE_11);
    addEnum(THREAD_AFFINITY_CORE_12);
    addEnum(THREAD_AFFINITY_CORE_13);
    addEnum(THREAD_AFFINITY_CORE_14);
    addEnum(THREAD_AFFINITY_CORE_15);
    addEnum(MAX_CHANNEL_WIDTH);
    addEnum(MAX_SYSTEMS);
    addEnum(MAX_LISTENERS);
    addEnum(REVERB_MAXINSTANCES);
    addEnum(STUDIO_LOAD_MEMORY_ALIGNMENT);
    addEnum(STUDIO_INIT_NORMAL);
    addEnum(STUDIO_INIT_LIVEUPDATE);
    addEnum(STUDIO_INIT_ALLOW_MISSING_PLUGINS);
    addEnum(STUDIO_INIT_SYNCHRONOUS_UPDATE);
    addEnum(STUDIO_INIT_DEFERRED_CALLBACKS);
    addEnum(STUDIO_INIT_LOAD_FROM_UPDATE);
    addEnum(STUDIO_INIT_MEMORY_TRACKING);
    addEnum(STUDIO_PARAMETER_READONLY);
    addEnum(STUDIO_PARAMETER_AUTOMATIC);
    addEnum(STUDIO_PARAMETER_GLOBAL);
    addEnum(STUDIO_PARAMETER_DISCRETE);
    addEnum(STUDIO_PARAMETER_LABELED);
    addEnum(STUDIO_SYSTEM_CALLBACK_PREUPDATE);
    addEnum(STUDIO_SYSTEM_CALLBACK_POSTUPDATE);
    addEnum(STUDIO_SYSTEM_CALLBACK_BANK_UNLOAD);
    addEnum(STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_CONNECTED);
    addEnum(STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_DISCONNECTED);
    addEnum(STUDIO_SYSTEM_CALLBACK_ALL);
    addEnum(STUDIO_EVENT_CALLBACK_CREATED);
    addEnum(STUDIO_EVENT_CALLBACK_DESTROYED);
    addEnum(STUDIO_EVENT_CALLBACK_STARTING);
    addEnum(STUDIO_EVENT_CALLBACK_STARTED);
    addEnum(STUDIO_EVENT_CALLBACK_RESTARTED);
    addEnum(STUDIO_EVENT_CALLBACK_STOPPED);
    addEnum(STUDIO_EVENT_CALLBACK_START_FAILED);
    addEnum(STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND);
    addEnum(STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND);
    addEnum(STUDIO_EVENT_CALLBACK_PLUGIN_CREATED);
    addEnum(STUDIO_EVENT_CALLBACK_PLUGIN_DESTROYED);
    addEnum(STUDIO_EVENT_CALLBACK_TIMELINE_MARKER);
    addEnum(STUDIO_EVENT_CALLBACK_TIMELINE_BEAT);
    addEnum(STUDIO_EVENT_CALLBACK_SOUND_PLAYED);
    addEnum(STUDIO_EVENT_CALLBACK_SOUND_STOPPED);
    addEnum(STUDIO_EVENT_CALLBACK_REAL_TO_VIRTUAL);
    addEnum(STUDIO_EVENT_CALLBACK_VIRTUAL_TO_REAL);
    addEnum(STUDIO_EVENT_CALLBACK_START_EVENT_COMMAND);
    addEnum(STUDIO_EVENT_CALLBACK_NESTED_TIMELINE_BEAT);
    addEnum(STUDIO_EVENT_CALLBACK_ALL);
    addEnum(STUDIO_LOAD_BANK_NORMAL);
    addEnum(STUDIO_LOAD_BANK_NONBLOCKING);
    addEnum(STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES);
    addEnum(STUDIO_LOAD_BANK_UNENCRYPTED);
    addEnum(STUDIO_COMMANDCAPTURE_NORMAL);
    addEnum(STUDIO_COMMANDCAPTURE_FILEFLUSH);
    addEnum(STUDIO_COMMANDCAPTURE_SKIP_INITIAL_STATE);
    addEnum(STUDIO_COMMANDREPLAY_NORMAL);
    addEnum(STUDIO_COMMANDREPLAY_SKIP_CLEANUP);
    addEnum(STUDIO_COMMANDREPLAY_FAST_FORWARD);
    addEnum(STUDIO_COMMANDREPLAY_SKIP_BANK_LOAD);
    

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

    beginStruct(FMOD_SYSTEM);
        addClassEquality(FMOD_SYSTEM);
        #ifdef FMODExt_func_FMOD_System_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetOutput
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetOutput);
        lua_setfield(L, -4, "set_output");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetOutput
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetOutput);
        lua_setfield(L, -4, "get_output");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetNumDrivers
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetNumDrivers);
        lua_setfield(L, -4, "get_num_drivers");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetDriverInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetDriverInfo);
        lua_setfield(L, -4, "get_driver_info");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetDriver
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetDriver);
        lua_setfield(L, -4, "set_driver");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetDriver
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetDriver);
        lua_setfield(L, -4, "get_driver");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetSoftwareChannels
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetSoftwareChannels);
        lua_setfield(L, -4, "set_software_channels");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetSoftwareChannels
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetSoftwareChannels);
        lua_setfield(L, -4, "get_software_channels");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetSoftwareFormat
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetSoftwareFormat);
        lua_setfield(L, -4, "set_software_format");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetSoftwareFormat
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetSoftwareFormat);
        lua_setfield(L, -4, "get_software_format");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetDSPBufferSize
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetDSPBufferSize);
        lua_setfield(L, -4, "set_dsp_buffer_size");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetDSPBufferSize
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetDSPBufferSize);
        lua_setfield(L, -4, "get_dsp_buffer_size");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetFileSystem
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetFileSystem);
        lua_setfield(L, -4, "set_file_system");
        #endif
        #ifdef FMODExt_func_FMOD_System_AttachFileSystem
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_AttachFileSystem);
        lua_setfield(L, -4, "attach_file_system");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetAdvancedSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetAdvancedSettings);
        lua_setfield(L, -4, "set_advanced_settings");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetAdvancedSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetAdvancedSettings);
        lua_setfield(L, -4, "get_advanced_settings");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetCallback);
        lua_setfield(L, -4, "set_callback");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetPluginPath
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetPluginPath);
        lua_setfield(L, -4, "set_plugin_path");
        #endif
        #ifdef FMODExt_func_FMOD_System_LoadPlugin
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_LoadPlugin);
        lua_setfield(L, -4, "load_plugin");
        #endif
        #ifdef FMODExt_func_FMOD_System_UnloadPlugin
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_UnloadPlugin);
        lua_setfield(L, -4, "unload_plugin");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetNumNestedPlugins
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetNumNestedPlugins);
        lua_setfield(L, -4, "get_num_nested_plugins");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetNestedPlugin
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetNestedPlugin);
        lua_setfield(L, -4, "get_nested_plugin");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetNumPlugins
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetNumPlugins);
        lua_setfield(L, -4, "get_num_plugins");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetPluginHandle
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetPluginHandle);
        lua_setfield(L, -4, "get_plugin_handle");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetPluginInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetPluginInfo);
        lua_setfield(L, -4, "get_plugin_info");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetOutputByPlugin
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetOutputByPlugin);
        lua_setfield(L, -4, "set_output_by_plugin");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetOutputByPlugin
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetOutputByPlugin);
        lua_setfield(L, -4, "get_output_by_plugin");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateDSPByPlugin
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateDSPByPlugin);
        lua_setfield(L, -4, "create_dsp_by_plugin");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetDSPInfoByPlugin
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetDSPInfoByPlugin);
        lua_setfield(L, -4, "get_dsp_info_by_plugin");
        #endif
        #ifdef FMODExt_func_FMOD_System_RegisterCodec
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_RegisterCodec);
        lua_setfield(L, -4, "register_codec");
        #endif
        #ifdef FMODExt_func_FMOD_System_RegisterDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_RegisterDSP);
        lua_setfield(L, -4, "register_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_System_RegisterOutput
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_RegisterOutput);
        lua_setfield(L, -4, "register_output");
        #endif
        #ifdef FMODExt_func_FMOD_System_Init
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Init);
        lua_setfield(L, -4, "init");
        #endif
        #ifdef FMODExt_func_FMOD_System_Close
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Close);
        lua_setfield(L, -4, "close");
        #endif
        #ifdef FMODExt_func_FMOD_System_Update
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Update);
        lua_setfield(L, -4, "update");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetSpeakerPosition
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetSpeakerPosition);
        lua_setfield(L, -4, "set_speaker_position");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetSpeakerPosition
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetSpeakerPosition);
        lua_setfield(L, -4, "get_speaker_position");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetStreamBufferSize
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetStreamBufferSize);
        lua_setfield(L, -4, "set_stream_buffer_size");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetStreamBufferSize
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetStreamBufferSize);
        lua_setfield(L, -4, "get_stream_buffer_size");
        #endif
        #ifdef FMODExt_func_FMOD_System_Set3DSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Set3DSettings);
        lua_setfield(L, -4, "set_3d_settings");
        #endif
        #ifdef FMODExt_func_FMOD_System_Get3DSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Get3DSettings);
        lua_setfield(L, -4, "get_3d_settings");
        #endif
        #ifdef FMODExt_func_FMOD_System_Set3DNumListeners
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Set3DNumListeners);
        lua_setfield(L, -4, "set_3d_num_listeners");
        #endif
        #ifdef FMODExt_func_FMOD_System_Get3DNumListeners
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Get3DNumListeners);
        lua_setfield(L, -4, "get_3d_num_listeners");
        #endif
        #ifdef FMODExt_func_FMOD_System_Set3DListenerAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Set3DListenerAttributes);
        lua_setfield(L, -4, "set_3d_listener_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_System_Get3DListenerAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Get3DListenerAttributes);
        lua_setfield(L, -4, "get_3d_listener_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_System_Set3DRolloffCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Set3DRolloffCallback);
        lua_setfield(L, -4, "set_3d_rolloff_callback");
        #endif
        #ifdef FMODExt_func_FMOD_System_MixerSuspend
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_MixerSuspend);
        lua_setfield(L, -4, "mixer_suspend");
        #endif
        #ifdef FMODExt_func_FMOD_System_MixerResume
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_MixerResume);
        lua_setfield(L, -4, "mixer_resume");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetDefaultMixMatrix
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetDefaultMixMatrix);
        lua_setfield(L, -4, "get_default_mix_matrix");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetSpeakerModeChannels
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetSpeakerModeChannels);
        lua_setfield(L, -4, "get_speaker_mode_channels");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetVersion
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetVersion);
        lua_setfield(L, -4, "get_version");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetOutputHandle
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetOutputHandle);
        lua_setfield(L, -4, "get_output_handle");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetChannelsPlaying
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetChannelsPlaying);
        lua_setfield(L, -4, "get_channels_playing");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetCPUUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetCPUUsage);
        lua_setfield(L, -4, "get_cpu_usage");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetFileUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetFileUsage);
        lua_setfield(L, -4, "get_file_usage");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateSound
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateSound);
        lua_setfield(L, -4, "create_sound");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateStream
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateStream);
        lua_setfield(L, -4, "create_stream");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateDSP);
        lua_setfield(L, -4, "create_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateDSPByType
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateDSPByType);
        lua_setfield(L, -4, "create_dsp_by_type");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateDSPConnection
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateDSPConnection);
        lua_setfield(L, -4, "create_dsp_connection");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateChannelGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateChannelGroup);
        lua_setfield(L, -4, "create_channel_group");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateSoundGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateSoundGroup);
        lua_setfield(L, -4, "create_sound_group");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateReverb3D
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateReverb3D);
        lua_setfield(L, -4, "create_reverb_3d");
        #endif
        #ifdef FMODExt_func_FMOD_System_PlaySound
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_PlaySound);
        lua_setfield(L, -4, "play_sound");
        #endif
        #ifdef FMODExt_func_FMOD_System_PlayDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_PlayDSP);
        lua_setfield(L, -4, "play_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetChannel
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetChannel);
        lua_setfield(L, -4, "get_channel");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetDSPInfoByType
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetDSPInfoByType);
        lua_setfield(L, -4, "get_dsp_info_by_type");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetMasterChannelGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetMasterChannelGroup);
        lua_setfield(L, -4, "get_master_channel_group");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetMasterSoundGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetMasterSoundGroup);
        lua_setfield(L, -4, "get_master_sound_group");
        #endif
        #ifdef FMODExt_func_FMOD_System_AttachChannelGroupToPort
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_AttachChannelGroupToPort);
        lua_setfield(L, -4, "attach_channel_group_to_port");
        #endif
        #ifdef FMODExt_func_FMOD_System_DetachChannelGroupFromPort
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_DetachChannelGroupFromPort);
        lua_setfield(L, -4, "detach_channel_group_from_port");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetReverbProperties
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetReverbProperties);
        lua_setfield(L, -4, "set_reverb_properties");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetReverbProperties
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetReverbProperties);
        lua_setfield(L, -4, "get_reverb_properties");
        #endif
        #ifdef FMODExt_func_FMOD_System_LockDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_LockDSP);
        lua_setfield(L, -4, "lock_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_System_UnlockDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_UnlockDSP);
        lua_setfield(L, -4, "unlock_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetRecordNumDrivers
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetRecordNumDrivers);
        lua_setfield(L, -4, "get_record_num_drivers");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetRecordDriverInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetRecordDriverInfo);
        lua_setfield(L, -4, "get_record_driver_info");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetRecordPosition
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetRecordPosition);
        lua_setfield(L, -4, "get_record_position");
        #endif
        #ifdef FMODExt_func_FMOD_System_RecordStart
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_RecordStart);
        lua_setfield(L, -4, "record_start");
        #endif
        #ifdef FMODExt_func_FMOD_System_RecordStop
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_RecordStop);
        lua_setfield(L, -4, "record_stop");
        #endif
        #ifdef FMODExt_func_FMOD_System_IsRecording
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_IsRecording);
        lua_setfield(L, -4, "is_recording");
        #endif
        #ifdef FMODExt_func_FMOD_System_CreateGeometry
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_CreateGeometry);
        lua_setfield(L, -4, "create_geometry");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetGeometrySettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetGeometrySettings);
        lua_setfield(L, -4, "set_geometry_settings");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetGeometrySettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetGeometrySettings);
        lua_setfield(L, -4, "get_geometry_settings");
        #endif
        #ifdef FMODExt_func_FMOD_System_LoadGeometry
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_LoadGeometry);
        lua_setfield(L, -4, "load_geometry");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetGeometryOcclusion
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetGeometryOcclusion);
        lua_setfield(L, -4, "get_geometry_occlusion");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetNetworkProxy
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetNetworkProxy);
        lua_setfield(L, -4, "set_network_proxy");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetNetworkProxy
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetNetworkProxy);
        lua_setfield(L, -4, "get_network_proxy");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetNetworkTimeout
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetNetworkTimeout);
        lua_setfield(L, -4, "set_network_timeout");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetNetworkTimeout
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetNetworkTimeout);
        lua_setfield(L, -4, "get_network_timeout");
        #endif
        #ifdef FMODExt_func_FMOD_System_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_System_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_extras_FMOD_SYSTEM
        FMODExt_extras_FMOD_SYSTEM
        #endif
    endStruct();
    beginStruct(FMOD_SOUND);
        addClassEquality(FMOD_SOUND);
        #ifdef FMODExt_func_FMOD_Sound_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetSystemObject
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetSystemObject);
        lua_setfield(L, -4, "get_system_object");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_Lock
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_Lock);
        lua_setfield(L, -4, "lock");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_Unlock
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_Unlock);
        lua_setfield(L, -4, "unlock");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_SetDefaults
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_SetDefaults);
        lua_setfield(L, -4, "set_defaults");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetDefaults
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetDefaults);
        lua_setfield(L, -4, "get_defaults");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_Set3DMinMaxDistance
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_Set3DMinMaxDistance);
        lua_setfield(L, -4, "set_3d_min_max_distance");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_Get3DMinMaxDistance
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_Get3DMinMaxDistance);
        lua_setfield(L, -4, "get_3d_min_max_distance");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_Set3DConeSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_Set3DConeSettings);
        lua_setfield(L, -4, "set_3d_cone_settings");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_Get3DConeSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_Get3DConeSettings);
        lua_setfield(L, -4, "get_3d_cone_settings");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_Set3DCustomRolloff
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_Set3DCustomRolloff);
        lua_setfield(L, -4, "set_3d_custom_rolloff");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_Get3DCustomRolloff
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_Get3DCustomRolloff);
        lua_setfield(L, -4, "get_3d_custom_rolloff");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetSubSound
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetSubSound);
        lua_setfield(L, -4, "get_sub_sound");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetSubSoundParent
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetSubSoundParent);
        lua_setfield(L, -4, "get_sub_sound_parent");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetName
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetName);
        lua_setfield(L, -4, "get_name");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetLength
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetLength);
        lua_setfield(L, -4, "get_length");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetFormat
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetFormat);
        lua_setfield(L, -4, "get_format");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetNumSubSounds
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetNumSubSounds);
        lua_setfield(L, -4, "get_num_sub_sounds");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetNumTags
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetNumTags);
        lua_setfield(L, -4, "get_num_tags");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetTag
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetTag);
        lua_setfield(L, -4, "get_tag");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetOpenState
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetOpenState);
        lua_setfield(L, -4, "get_open_state");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_ReadData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_ReadData);
        lua_setfield(L, -4, "read_data");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_SeekData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_SeekData);
        lua_setfield(L, -4, "seek_data");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_SetSoundGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_SetSoundGroup);
        lua_setfield(L, -4, "set_sound_group");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetSoundGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetSoundGroup);
        lua_setfield(L, -4, "get_sound_group");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetNumSyncPoints
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetNumSyncPoints);
        lua_setfield(L, -4, "get_num_sync_points");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetSyncPoint
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetSyncPoint);
        lua_setfield(L, -4, "get_sync_point");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetSyncPointInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetSyncPointInfo);
        lua_setfield(L, -4, "get_sync_point_info");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_AddSyncPoint
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_AddSyncPoint);
        lua_setfield(L, -4, "add_sync_point");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_DeleteSyncPoint
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_DeleteSyncPoint);
        lua_setfield(L, -4, "delete_sync_point");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_SetMode
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_SetMode);
        lua_setfield(L, -4, "set_mode");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetMode
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetMode);
        lua_setfield(L, -4, "get_mode");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_SetLoopCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_SetLoopCount);
        lua_setfield(L, -4, "set_loop_count");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetLoopCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetLoopCount);
        lua_setfield(L, -4, "get_loop_count");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_SetLoopPoints
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_SetLoopPoints);
        lua_setfield(L, -4, "set_loop_points");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetLoopPoints
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetLoopPoints);
        lua_setfield(L, -4, "get_loop_points");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetMusicNumChannels
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetMusicNumChannels);
        lua_setfield(L, -4, "get_music_num_channels");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_SetMusicChannelVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_SetMusicChannelVolume);
        lua_setfield(L, -4, "set_music_channel_volume");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetMusicChannelVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetMusicChannelVolume);
        lua_setfield(L, -4, "get_music_channel_volume");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_SetMusicSpeed
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_SetMusicSpeed);
        lua_setfield(L, -4, "set_music_speed");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetMusicSpeed
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetMusicSpeed);
        lua_setfield(L, -4, "get_music_speed");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Sound_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Sound_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_extras_FMOD_SOUND
        FMODExt_extras_FMOD_SOUND
        #endif
    endStruct();
    beginStruct(FMOD_CHANNELCONTROL);
        addClassEquality(FMOD_CHANNELCONTROL);
        #ifdef FMODExt_extras_FMOD_CHANNELCONTROL
        FMODExt_extras_FMOD_CHANNELCONTROL
        #endif
    endStruct();
    beginStruct(FMOD_CHANNEL);
        addClassEquality(FMOD_CHANNEL);
        #ifdef FMODExt_func_FMOD_Channel_GetSystemObject
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetSystemObject);
        lua_setfield(L, -4, "get_system_object");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Stop
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Stop);
        lua_setfield(L, -4, "stop");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetPaused);
        lua_setfield(L, -4, "set_paused");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetPaused);
        lua_setfield(L, -4, "get_paused");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetVolume);
        lua_setfield(L, -4, "set_volume");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetVolume);
        lua_setfield(L, -4, "get_volume");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetVolumeRamp
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetVolumeRamp);
        lua_setfield(L, -4, "set_volume_ramp");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetVolumeRamp
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetVolumeRamp);
        lua_setfield(L, -4, "get_volume_ramp");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetAudibility
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetAudibility);
        lua_setfield(L, -4, "get_audibility");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetPitch
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetPitch);
        lua_setfield(L, -4, "set_pitch");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetPitch
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetPitch);
        lua_setfield(L, -4, "get_pitch");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetMute
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetMute);
        lua_setfield(L, -4, "set_mute");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetMute
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetMute);
        lua_setfield(L, -4, "get_mute");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetReverbProperties
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetReverbProperties);
        lua_setfield(L, -4, "set_reverb_properties");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetReverbProperties
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetReverbProperties);
        lua_setfield(L, -4, "get_reverb_properties");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetLowPassGain
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetLowPassGain);
        lua_setfield(L, -4, "set_low_pass_gain");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetLowPassGain
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetLowPassGain);
        lua_setfield(L, -4, "get_low_pass_gain");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetMode
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetMode);
        lua_setfield(L, -4, "set_mode");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetMode
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetMode);
        lua_setfield(L, -4, "get_mode");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetCallback);
        lua_setfield(L, -4, "set_callback");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_IsPlaying
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_IsPlaying);
        lua_setfield(L, -4, "is_playing");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetPan
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetPan);
        lua_setfield(L, -4, "set_pan");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetMixLevelsOutput
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetMixLevelsOutput);
        lua_setfield(L, -4, "set_mix_levels_output");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetMixLevelsInput
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetMixLevelsInput);
        lua_setfield(L, -4, "set_mix_levels_input");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetMixMatrix
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetMixMatrix);
        lua_setfield(L, -4, "set_mix_matrix");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetMixMatrix
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetMixMatrix);
        lua_setfield(L, -4, "get_mix_matrix");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetDSPClock
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetDSPClock);
        lua_setfield(L, -4, "get_dsp_clock");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetDelay
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetDelay);
        lua_setfield(L, -4, "set_delay");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetDelay
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetDelay);
        lua_setfield(L, -4, "get_delay");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_AddFadePoint
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_AddFadePoint);
        lua_setfield(L, -4, "add_fade_point");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetFadePointRamp
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetFadePointRamp);
        lua_setfield(L, -4, "set_fade_point_ramp");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_RemoveFadePoints
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_RemoveFadePoints);
        lua_setfield(L, -4, "remove_fade_points");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetFadePoints
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetFadePoints);
        lua_setfield(L, -4, "get_fade_points");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetDSP);
        lua_setfield(L, -4, "get_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_AddDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_AddDSP);
        lua_setfield(L, -4, "add_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_RemoveDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_RemoveDSP);
        lua_setfield(L, -4, "remove_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetNumDSPs
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetNumDSPs);
        lua_setfield(L, -4, "get_num_ds_ps");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetDSPIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetDSPIndex);
        lua_setfield(L, -4, "set_dsp_index");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetDSPIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetDSPIndex);
        lua_setfield(L, -4, "get_dsp_index");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DAttributes);
        lua_setfield(L, -4, "set_3d_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DAttributes);
        lua_setfield(L, -4, "get_3d_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DMinMaxDistance
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DMinMaxDistance);
        lua_setfield(L, -4, "set_3d_min_max_distance");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DMinMaxDistance
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DMinMaxDistance);
        lua_setfield(L, -4, "get_3d_min_max_distance");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DConeSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DConeSettings);
        lua_setfield(L, -4, "set_3d_cone_settings");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DConeSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DConeSettings);
        lua_setfield(L, -4, "get_3d_cone_settings");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DConeOrientation
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DConeOrientation);
        lua_setfield(L, -4, "set_3d_cone_orientation");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DConeOrientation
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DConeOrientation);
        lua_setfield(L, -4, "get_3d_cone_orientation");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DCustomRolloff
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DCustomRolloff);
        lua_setfield(L, -4, "set_3d_custom_rolloff");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DCustomRolloff
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DCustomRolloff);
        lua_setfield(L, -4, "get_3d_custom_rolloff");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DOcclusion
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DOcclusion);
        lua_setfield(L, -4, "set_3d_occlusion");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DOcclusion
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DOcclusion);
        lua_setfield(L, -4, "get_3d_occlusion");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DSpread
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DSpread);
        lua_setfield(L, -4, "set_3d_spread");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DSpread
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DSpread);
        lua_setfield(L, -4, "get_3d_spread");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DLevel);
        lua_setfield(L, -4, "set_3d_level");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DLevel);
        lua_setfield(L, -4, "get_3d_level");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DDopplerLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DDopplerLevel);
        lua_setfield(L, -4, "set_3d_doppler_level");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DDopplerLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DDopplerLevel);
        lua_setfield(L, -4, "get_3d_doppler_level");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Set3DDistanceFilter
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Set3DDistanceFilter);
        lua_setfield(L, -4, "set_3d_distance_filter");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_Get3DDistanceFilter
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_Get3DDistanceFilter);
        lua_setfield(L, -4, "get_3d_distance_filter");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetFrequency
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetFrequency);
        lua_setfield(L, -4, "set_frequency");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetFrequency
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetFrequency);
        lua_setfield(L, -4, "get_frequency");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetPriority
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetPriority);
        lua_setfield(L, -4, "set_priority");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetPriority
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetPriority);
        lua_setfield(L, -4, "get_priority");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetPosition
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetPosition);
        lua_setfield(L, -4, "set_position");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetPosition
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetPosition);
        lua_setfield(L, -4, "get_position");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetChannelGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetChannelGroup);
        lua_setfield(L, -4, "set_channel_group");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetChannelGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetChannelGroup);
        lua_setfield(L, -4, "get_channel_group");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetLoopCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetLoopCount);
        lua_setfield(L, -4, "set_loop_count");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetLoopCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetLoopCount);
        lua_setfield(L, -4, "get_loop_count");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_SetLoopPoints
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_SetLoopPoints);
        lua_setfield(L, -4, "set_loop_points");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetLoopPoints
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetLoopPoints);
        lua_setfield(L, -4, "get_loop_points");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_IsVirtual
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_IsVirtual);
        lua_setfield(L, -4, "is_virtual");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetCurrentSound
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetCurrentSound);
        lua_setfield(L, -4, "get_current_sound");
        #endif
        #ifdef FMODExt_func_FMOD_Channel_GetIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Channel_GetIndex);
        lua_setfield(L, -4, "get_index");
        #endif
        #ifdef FMODExt_extras_FMOD_CHANNEL
        FMODExt_extras_FMOD_CHANNEL
        #endif
    endStruct();
    beginStruct(FMOD_CHANNELGROUP);
        addClassEquality(FMOD_CHANNELGROUP);
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetSystemObject
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetSystemObject);
        lua_setfield(L, -4, "get_system_object");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Stop
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Stop);
        lua_setfield(L, -4, "stop");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetPaused);
        lua_setfield(L, -4, "set_paused");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetPaused);
        lua_setfield(L, -4, "get_paused");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetVolume);
        lua_setfield(L, -4, "set_volume");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetVolume);
        lua_setfield(L, -4, "get_volume");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetVolumeRamp
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetVolumeRamp);
        lua_setfield(L, -4, "set_volume_ramp");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetVolumeRamp
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetVolumeRamp);
        lua_setfield(L, -4, "get_volume_ramp");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetAudibility
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetAudibility);
        lua_setfield(L, -4, "get_audibility");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetPitch
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetPitch);
        lua_setfield(L, -4, "set_pitch");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetPitch
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetPitch);
        lua_setfield(L, -4, "get_pitch");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetMute
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetMute);
        lua_setfield(L, -4, "set_mute");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetMute
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetMute);
        lua_setfield(L, -4, "get_mute");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetReverbProperties
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetReverbProperties);
        lua_setfield(L, -4, "set_reverb_properties");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetReverbProperties
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetReverbProperties);
        lua_setfield(L, -4, "get_reverb_properties");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetLowPassGain
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetLowPassGain);
        lua_setfield(L, -4, "set_low_pass_gain");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetLowPassGain
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetLowPassGain);
        lua_setfield(L, -4, "get_low_pass_gain");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetMode
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetMode);
        lua_setfield(L, -4, "set_mode");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetMode
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetMode);
        lua_setfield(L, -4, "get_mode");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetCallback);
        lua_setfield(L, -4, "set_callback");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_IsPlaying
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_IsPlaying);
        lua_setfield(L, -4, "is_playing");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetPan
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetPan);
        lua_setfield(L, -4, "set_pan");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetMixLevelsOutput
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetMixLevelsOutput);
        lua_setfield(L, -4, "set_mix_levels_output");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetMixLevelsInput
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetMixLevelsInput);
        lua_setfield(L, -4, "set_mix_levels_input");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetMixMatrix
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetMixMatrix);
        lua_setfield(L, -4, "set_mix_matrix");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetMixMatrix
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetMixMatrix);
        lua_setfield(L, -4, "get_mix_matrix");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetDSPClock
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetDSPClock);
        lua_setfield(L, -4, "get_dsp_clock");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetDelay
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetDelay);
        lua_setfield(L, -4, "set_delay");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetDelay
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetDelay);
        lua_setfield(L, -4, "get_delay");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_AddFadePoint
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_AddFadePoint);
        lua_setfield(L, -4, "add_fade_point");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetFadePointRamp
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetFadePointRamp);
        lua_setfield(L, -4, "set_fade_point_ramp");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_RemoveFadePoints
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_RemoveFadePoints);
        lua_setfield(L, -4, "remove_fade_points");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetFadePoints
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetFadePoints);
        lua_setfield(L, -4, "get_fade_points");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetDSP);
        lua_setfield(L, -4, "get_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_AddDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_AddDSP);
        lua_setfield(L, -4, "add_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_RemoveDSP
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_RemoveDSP);
        lua_setfield(L, -4, "remove_dsp");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetNumDSPs
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetNumDSPs);
        lua_setfield(L, -4, "get_num_ds_ps");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetDSPIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetDSPIndex);
        lua_setfield(L, -4, "set_dsp_index");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetDSPIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetDSPIndex);
        lua_setfield(L, -4, "get_dsp_index");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DAttributes);
        lua_setfield(L, -4, "set_3d_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DAttributes);
        lua_setfield(L, -4, "get_3d_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DMinMaxDistance
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DMinMaxDistance);
        lua_setfield(L, -4, "set_3d_min_max_distance");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DMinMaxDistance
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DMinMaxDistance);
        lua_setfield(L, -4, "get_3d_min_max_distance");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DConeSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DConeSettings);
        lua_setfield(L, -4, "set_3d_cone_settings");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DConeSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DConeSettings);
        lua_setfield(L, -4, "get_3d_cone_settings");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DConeOrientation
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DConeOrientation);
        lua_setfield(L, -4, "set_3d_cone_orientation");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DConeOrientation
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DConeOrientation);
        lua_setfield(L, -4, "get_3d_cone_orientation");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DCustomRolloff
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DCustomRolloff);
        lua_setfield(L, -4, "set_3d_custom_rolloff");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DCustomRolloff
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DCustomRolloff);
        lua_setfield(L, -4, "get_3d_custom_rolloff");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DOcclusion
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DOcclusion);
        lua_setfield(L, -4, "set_3d_occlusion");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DOcclusion
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DOcclusion);
        lua_setfield(L, -4, "get_3d_occlusion");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DSpread
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DSpread);
        lua_setfield(L, -4, "set_3d_spread");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DSpread
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DSpread);
        lua_setfield(L, -4, "get_3d_spread");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DLevel);
        lua_setfield(L, -4, "set_3d_level");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DLevel);
        lua_setfield(L, -4, "get_3d_level");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DDopplerLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DDopplerLevel);
        lua_setfield(L, -4, "set_3d_doppler_level");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DDopplerLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DDopplerLevel);
        lua_setfield(L, -4, "get_3d_doppler_level");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Set3DDistanceFilter
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Set3DDistanceFilter);
        lua_setfield(L, -4, "set_3d_distance_filter");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Get3DDistanceFilter
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Get3DDistanceFilter);
        lua_setfield(L, -4, "get_3d_distance_filter");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_AddGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_AddGroup);
        lua_setfield(L, -4, "add_group");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetNumGroups
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetNumGroups);
        lua_setfield(L, -4, "get_num_groups");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetGroup);
        lua_setfield(L, -4, "get_group");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetParentGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetParentGroup);
        lua_setfield(L, -4, "get_parent_group");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetName
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetName);
        lua_setfield(L, -4, "get_name");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetNumChannels
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetNumChannels);
        lua_setfield(L, -4, "get_num_channels");
        #endif
        #ifdef FMODExt_func_FMOD_ChannelGroup_GetChannel
        lua_pushcfunction(L, &FMODExt_func_FMOD_ChannelGroup_GetChannel);
        lua_setfield(L, -4, "get_channel");
        #endif
        #ifdef FMODExt_extras_FMOD_CHANNELGROUP
        FMODExt_extras_FMOD_CHANNELGROUP
        #endif
    endStruct();
    beginStruct(FMOD_SOUNDGROUP);
        addClassEquality(FMOD_SOUNDGROUP);
        #ifdef FMODExt_func_FMOD_SoundGroup_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetSystemObject
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetSystemObject);
        lua_setfield(L, -4, "get_system_object");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_SetMaxAudible
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_SetMaxAudible);
        lua_setfield(L, -4, "set_max_audible");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetMaxAudible
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetMaxAudible);
        lua_setfield(L, -4, "get_max_audible");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_SetMaxAudibleBehavior
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_SetMaxAudibleBehavior);
        lua_setfield(L, -4, "set_max_audible_behavior");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetMaxAudibleBehavior
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetMaxAudibleBehavior);
        lua_setfield(L, -4, "get_max_audible_behavior");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_SetMuteFadeSpeed
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_SetMuteFadeSpeed);
        lua_setfield(L, -4, "set_mute_fade_speed");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetMuteFadeSpeed
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetMuteFadeSpeed);
        lua_setfield(L, -4, "get_mute_fade_speed");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_SetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_SetVolume);
        lua_setfield(L, -4, "set_volume");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetVolume);
        lua_setfield(L, -4, "get_volume");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_Stop
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_Stop);
        lua_setfield(L, -4, "stop");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetName
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetName);
        lua_setfield(L, -4, "get_name");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetNumSounds
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetNumSounds);
        lua_setfield(L, -4, "get_num_sounds");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetSound
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetSound);
        lua_setfield(L, -4, "get_sound");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetNumPlaying
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetNumPlaying);
        lua_setfield(L, -4, "get_num_playing");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_SoundGroup_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_SoundGroup_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_extras_FMOD_SOUNDGROUP
        FMODExt_extras_FMOD_SOUNDGROUP
        #endif
    endStruct();
    beginStruct(FMOD_REVERB3D);
        addClassEquality(FMOD_REVERB3D);
        #ifdef FMODExt_func_FMOD_Reverb3D_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_Reverb3D_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_Reverb3D_Set3DAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Reverb3D_Set3DAttributes);
        lua_setfield(L, -4, "set_3d_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Reverb3D_Get3DAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Reverb3D_Get3DAttributes);
        lua_setfield(L, -4, "get_3d_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Reverb3D_SetProperties
        lua_pushcfunction(L, &FMODExt_func_FMOD_Reverb3D_SetProperties);
        lua_setfield(L, -4, "set_properties");
        #endif
        #ifdef FMODExt_func_FMOD_Reverb3D_GetProperties
        lua_pushcfunction(L, &FMODExt_func_FMOD_Reverb3D_GetProperties);
        lua_setfield(L, -4, "get_properties");
        #endif
        #ifdef FMODExt_func_FMOD_Reverb3D_SetActive
        lua_pushcfunction(L, &FMODExt_func_FMOD_Reverb3D_SetActive);
        lua_setfield(L, -4, "set_active");
        #endif
        #ifdef FMODExt_func_FMOD_Reverb3D_GetActive
        lua_pushcfunction(L, &FMODExt_func_FMOD_Reverb3D_GetActive);
        lua_setfield(L, -4, "get_active");
        #endif
        #ifdef FMODExt_func_FMOD_Reverb3D_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Reverb3D_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Reverb3D_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Reverb3D_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_extras_FMOD_REVERB3D
        FMODExt_extras_FMOD_REVERB3D
        #endif
    endStruct();
    beginStruct(FMOD_DSP);
        addClassEquality(FMOD_DSP);
        #ifdef FMODExt_func_FMOD_DSP_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetSystemObject
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetSystemObject);
        lua_setfield(L, -4, "get_system_object");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_AddInput
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_AddInput);
        lua_setfield(L, -4, "add_input");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_DisconnectFrom
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_DisconnectFrom);
        lua_setfield(L, -4, "disconnect_from");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_DisconnectAll
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_DisconnectAll);
        lua_setfield(L, -4, "disconnect_all");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetNumInputs
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetNumInputs);
        lua_setfield(L, -4, "get_num_inputs");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetNumOutputs
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetNumOutputs);
        lua_setfield(L, -4, "get_num_outputs");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetInput
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetInput);
        lua_setfield(L, -4, "get_input");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetOutput
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetOutput);
        lua_setfield(L, -4, "get_output");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetActive
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetActive);
        lua_setfield(L, -4, "set_active");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetActive
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetActive);
        lua_setfield(L, -4, "get_active");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetBypass
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetBypass);
        lua_setfield(L, -4, "set_bypass");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetBypass
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetBypass);
        lua_setfield(L, -4, "get_bypass");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetWetDryMix
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetWetDryMix);
        lua_setfield(L, -4, "set_wet_dry_mix");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetWetDryMix
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetWetDryMix);
        lua_setfield(L, -4, "get_wet_dry_mix");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetChannelFormat
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetChannelFormat);
        lua_setfield(L, -4, "set_channel_format");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetChannelFormat
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetChannelFormat);
        lua_setfield(L, -4, "get_channel_format");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetOutputChannelFormat
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetOutputChannelFormat);
        lua_setfield(L, -4, "get_output_channel_format");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_Reset
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_Reset);
        lua_setfield(L, -4, "reset");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetCallback);
        lua_setfield(L, -4, "set_callback");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetParameterFloat
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetParameterFloat);
        lua_setfield(L, -4, "set_parameter_float");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetParameterInt
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetParameterInt);
        lua_setfield(L, -4, "set_parameter_int");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetParameterBool
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetParameterBool);
        lua_setfield(L, -4, "set_parameter_bool");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetParameterData
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetParameterData);
        lua_setfield(L, -4, "set_parameter_data");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetParameterFloat
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetParameterFloat);
        lua_setfield(L, -4, "get_parameter_float");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetParameterInt
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetParameterInt);
        lua_setfield(L, -4, "get_parameter_int");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetParameterBool
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetParameterBool);
        lua_setfield(L, -4, "get_parameter_bool");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetParameterData
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetParameterData);
        lua_setfield(L, -4, "get_parameter_data");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetNumParameters
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetNumParameters);
        lua_setfield(L, -4, "get_num_parameters");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetParameterInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetParameterInfo);
        lua_setfield(L, -4, "get_parameter_info");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetDataParameterIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetDataParameterIndex);
        lua_setfield(L, -4, "get_data_parameter_index");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_ShowConfigDialog
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_ShowConfigDialog);
        lua_setfield(L, -4, "show_config_dialog");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetInfo);
        lua_setfield(L, -4, "get_info");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetType
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetType);
        lua_setfield(L, -4, "get_type");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetIdle
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetIdle);
        lua_setfield(L, -4, "get_idle");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_SetMeteringEnabled
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_SetMeteringEnabled);
        lua_setfield(L, -4, "set_metering_enabled");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetMeteringEnabled
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetMeteringEnabled);
        lua_setfield(L, -4, "get_metering_enabled");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetMeteringInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetMeteringInfo);
        lua_setfield(L, -4, "get_metering_info");
        #endif
        #ifdef FMODExt_func_FMOD_DSP_GetCPUUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSP_GetCPUUsage);
        lua_setfield(L, -4, "get_cpu_usage");
        #endif
        #ifdef FMODExt_extras_FMOD_DSP
        FMODExt_extras_FMOD_DSP
        #endif
    endStruct();
    beginStruct(FMOD_DSPCONNECTION);
        addClassEquality(FMOD_DSPCONNECTION);
        #ifdef FMODExt_func_FMOD_DSPConnection_GetInput
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSPConnection_GetInput);
        lua_setfield(L, -4, "get_input");
        #endif
        #ifdef FMODExt_func_FMOD_DSPConnection_GetOutput
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSPConnection_GetOutput);
        lua_setfield(L, -4, "get_output");
        #endif
        #ifdef FMODExt_func_FMOD_DSPConnection_SetMix
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSPConnection_SetMix);
        lua_setfield(L, -4, "set_mix");
        #endif
        #ifdef FMODExt_func_FMOD_DSPConnection_GetMix
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSPConnection_GetMix);
        lua_setfield(L, -4, "get_mix");
        #endif
        #ifdef FMODExt_func_FMOD_DSPConnection_SetMixMatrix
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSPConnection_SetMixMatrix);
        lua_setfield(L, -4, "set_mix_matrix");
        #endif
        #ifdef FMODExt_func_FMOD_DSPConnection_GetMixMatrix
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSPConnection_GetMixMatrix);
        lua_setfield(L, -4, "get_mix_matrix");
        #endif
        #ifdef FMODExt_func_FMOD_DSPConnection_GetType
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSPConnection_GetType);
        lua_setfield(L, -4, "get_type");
        #endif
        #ifdef FMODExt_func_FMOD_DSPConnection_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSPConnection_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_DSPConnection_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_DSPConnection_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_extras_FMOD_DSPCONNECTION
        FMODExt_extras_FMOD_DSPCONNECTION
        #endif
    endStruct();
    beginStruct(FMOD_POLYGON);
        addClassEquality(FMOD_POLYGON);
        #ifdef FMODExt_extras_FMOD_POLYGON
        FMODExt_extras_FMOD_POLYGON
        #endif
    endStruct();
    beginStruct(FMOD_GEOMETRY);
        addClassEquality(FMOD_GEOMETRY);
        #ifdef FMODExt_func_FMOD_Geometry_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_AddPolygon
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_AddPolygon);
        lua_setfield(L, -4, "add_polygon");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetNumPolygons
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetNumPolygons);
        lua_setfield(L, -4, "get_num_polygons");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetMaxPolygons
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetMaxPolygons);
        lua_setfield(L, -4, "get_max_polygons");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetPolygonNumVertices
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetPolygonNumVertices);
        lua_setfield(L, -4, "get_polygon_num_vertices");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_SetPolygonVertex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_SetPolygonVertex);
        lua_setfield(L, -4, "set_polygon_vertex");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetPolygonVertex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetPolygonVertex);
        lua_setfield(L, -4, "get_polygon_vertex");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_SetPolygonAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_SetPolygonAttributes);
        lua_setfield(L, -4, "set_polygon_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetPolygonAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetPolygonAttributes);
        lua_setfield(L, -4, "get_polygon_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_SetActive
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_SetActive);
        lua_setfield(L, -4, "set_active");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetActive
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetActive);
        lua_setfield(L, -4, "get_active");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_SetRotation
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_SetRotation);
        lua_setfield(L, -4, "set_rotation");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetRotation
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetRotation);
        lua_setfield(L, -4, "get_rotation");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_SetPosition
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_SetPosition);
        lua_setfield(L, -4, "set_position");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetPosition
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetPosition);
        lua_setfield(L, -4, "get_position");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_SetScale
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_SetScale);
        lua_setfield(L, -4, "set_scale");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetScale
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetScale);
        lua_setfield(L, -4, "get_scale");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_Save
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_Save);
        lua_setfield(L, -4, "save");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Geometry_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Geometry_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_extras_FMOD_GEOMETRY
        FMODExt_extras_FMOD_GEOMETRY
        #endif
    endStruct();
    beginStruct(FMOD_SYNCPOINT);
        addClassEquality(FMOD_SYNCPOINT);
        #ifdef FMODExt_extras_FMOD_SYNCPOINT
        FMODExt_extras_FMOD_SYNCPOINT
        #endif
    endStruct();
    beginStruct(FMOD_ASYNCREADINFO);
        addStructEquality(FMOD_ASYNCREADINFO);
        addStructConstructor(FMOD_ASYNCREADINFO, "ASYNCREADINFO", -2);
        /* void* handle */
        /* unsigned int offset */
        addPropertyGetter(FMOD_ASYNCREADINFO, offset, unsigned_int);
        addPropertySetter(FMOD_ASYNCREADINFO, offset, unsigned_int);
        /* unsigned int sizebytes */
        addPropertyGetter(FMOD_ASYNCREADINFO, sizebytes, unsigned_int);
        addPropertySetter(FMOD_ASYNCREADINFO, sizebytes, unsigned_int);
        /* int priority */
        addPropertyGetter(FMOD_ASYNCREADINFO, priority, int);
        addPropertySetter(FMOD_ASYNCREADINFO, priority, int);
        /* void* userdata */
        /* void* buffer */
        /* unsigned int bytesread */
        addPropertyGetter(FMOD_ASYNCREADINFO, bytesread, unsigned_int);
        addPropertySetter(FMOD_ASYNCREADINFO, bytesread, unsigned_int);
        /* FMOD_FILE_ASYNCDONE_FUNC done */
        #ifdef FMODExt_extras_FMOD_ASYNCREADINFO
        FMODExt_extras_FMOD_ASYNCREADINFO
        #endif
    endStruct();
    beginStruct(FMOD_3D_ATTRIBUTES);
        addStructEquality(FMOD_3D_ATTRIBUTES);
        addStructConstructor(FMOD_3D_ATTRIBUTES, "_3D_ATTRIBUTES", -2);
        /* FMOD_VECTOR position */
        addPropertyGetter(FMOD_3D_ATTRIBUTES, position, FMOD_VECTOR);
        addPropertySetter(FMOD_3D_ATTRIBUTES, position, FMOD_VECTOR);
        /* FMOD_VECTOR velocity */
        addPropertyGetter(FMOD_3D_ATTRIBUTES, velocity, FMOD_VECTOR);
        addPropertySetter(FMOD_3D_ATTRIBUTES, velocity, FMOD_VECTOR);
        /* FMOD_VECTOR forward */
        addPropertyGetter(FMOD_3D_ATTRIBUTES, forward, FMOD_VECTOR);
        addPropertySetter(FMOD_3D_ATTRIBUTES, forward, FMOD_VECTOR);
        /* FMOD_VECTOR up */
        addPropertyGetter(FMOD_3D_ATTRIBUTES, up, FMOD_VECTOR);
        addPropertySetter(FMOD_3D_ATTRIBUTES, up, FMOD_VECTOR);
        #ifdef FMODExt_extras_FMOD_3D_ATTRIBUTES
        FMODExt_extras_FMOD_3D_ATTRIBUTES
        #endif
    endStruct();
    beginStruct(FMOD_GUID);
        addStructEquality(FMOD_GUID);
        addStructConstructor(FMOD_GUID, "GUID", -2);
        /* unsigned int Data1 */
        addPropertyGetter(FMOD_GUID, Data1, unsigned_int);
        addPropertySetter(FMOD_GUID, Data1, unsigned_int);
        /* unsigned short Data2 */
        addPropertyGetter(FMOD_GUID, Data2, unsigned_short);
        addPropertySetter(FMOD_GUID, Data2, unsigned_short);
        /* unsigned short Data3 */
        addPropertyGetter(FMOD_GUID, Data3, unsigned_short);
        addPropertySetter(FMOD_GUID, Data3, unsigned_short);
        /* __UNKNOWN__ Data4 */
        #ifdef FMODExt_extras_FMOD_GUID
        FMODExt_extras_FMOD_GUID
        #endif
    endStruct();
    beginStruct(FMOD_PLUGINLIST);
        addStructEquality(FMOD_PLUGINLIST);
        addStructConstructor(FMOD_PLUGINLIST, "PLUGINLIST", -2);
        /* FMOD_PLUGINTYPE type */
        addPropertyGetter(FMOD_PLUGINLIST, type, FMOD_PLUGINTYPE);
        addPropertySetter(FMOD_PLUGINLIST, type, FMOD_PLUGINTYPE);
        /* void* description */
        #ifdef FMODExt_extras_FMOD_PLUGINLIST
        FMODExt_extras_FMOD_PLUGINLIST
        #endif
    endStruct();
    beginStruct(FMOD_ADVANCEDSETTINGS);
        addStructEquality(FMOD_ADVANCEDSETTINGS);
        addStructConstructor(FMOD_ADVANCEDSETTINGS, "ADVANCEDSETTINGS", -2);
        /* int cbSize */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, cbSize, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, cbSize, int);
        /* int maxMPEGCodecs */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, maxMPEGCodecs, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, maxMPEGCodecs, int);
        /* int maxADPCMCodecs */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, maxADPCMCodecs, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, maxADPCMCodecs, int);
        /* int maxXMACodecs */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, maxXMACodecs, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, maxXMACodecs, int);
        /* int maxVorbisCodecs */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, maxVorbisCodecs, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, maxVorbisCodecs, int);
        /* int maxAT9Codecs */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, maxAT9Codecs, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, maxAT9Codecs, int);
        /* int maxFADPCMCodecs */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, maxFADPCMCodecs, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, maxFADPCMCodecs, int);
        /* int maxOpusCodecs */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, maxOpusCodecs, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, maxOpusCodecs, int);
        /* int ASIONumChannels */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, ASIONumChannels, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, ASIONumChannels, int);
        /* char** ASIOChannelList */
        /* FMOD_SPEAKER* ASIOSpeakerList */
        /* float vol0virtualvol */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, vol0virtualvol, float);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, vol0virtualvol, float);
        /* unsigned int defaultDecodeBufferSize */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, defaultDecodeBufferSize, unsigned_int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, defaultDecodeBufferSize, unsigned_int);
        /* unsigned short profilePort */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, profilePort, unsigned_short);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, profilePort, unsigned_short);
        /* unsigned int geometryMaxFadeTime */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, geometryMaxFadeTime, unsigned_int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, geometryMaxFadeTime, unsigned_int);
        /* float distanceFilterCenterFreq */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, distanceFilterCenterFreq, float);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, distanceFilterCenterFreq, float);
        /* int reverb3Dinstance */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, reverb3Dinstance, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, reverb3Dinstance, int);
        /* int DSPBufferPoolSize */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, DSPBufferPoolSize, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, DSPBufferPoolSize, int);
        /* FMOD_DSP_RESAMPLER resamplerMethod */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, resamplerMethod, FMOD_DSP_RESAMPLER);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, resamplerMethod, FMOD_DSP_RESAMPLER);
        /* unsigned int randomSeed */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, randomSeed, unsigned_int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, randomSeed, unsigned_int);
        /* int maxConvolutionThreads */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, maxConvolutionThreads, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, maxConvolutionThreads, int);
        /* int maxSpatialObjects */
        addPropertyGetter(FMOD_ADVANCEDSETTINGS, maxSpatialObjects, int);
        addPropertySetter(FMOD_ADVANCEDSETTINGS, maxSpatialObjects, int);
        #ifdef FMODExt_extras_FMOD_ADVANCEDSETTINGS
        FMODExt_extras_FMOD_ADVANCEDSETTINGS
        #endif
    endStruct();
    beginStruct(FMOD_TAG);
        addStructEquality(FMOD_TAG);
        addStructConstructor(FMOD_TAG, "TAG", -2);
        /* FMOD_TAGTYPE type */
        addPropertyGetter(FMOD_TAG, type, FMOD_TAGTYPE);
        addPropertySetter(FMOD_TAG, type, FMOD_TAGTYPE);
        /* FMOD_TAGDATATYPE datatype */
        addPropertyGetter(FMOD_TAG, datatype, FMOD_TAGDATATYPE);
        addPropertySetter(FMOD_TAG, datatype, FMOD_TAGDATATYPE);
        /* char* name */
        addPropertyGetter(FMOD_TAG, name, ptr_char);
        /* void* data */
        /* unsigned int datalen */
        addPropertyGetter(FMOD_TAG, datalen, unsigned_int);
        addPropertySetter(FMOD_TAG, datalen, unsigned_int);
        /* FMOD_BOOL updated */
        addPropertyGetter(FMOD_TAG, updated, FMOD_BOOL);
        addPropertySetter(FMOD_TAG, updated, FMOD_BOOL);
        #ifdef FMODExt_extras_FMOD_TAG
        FMODExt_extras_FMOD_TAG
        #endif
    endStruct();
    beginStruct(FMOD_CREATESOUNDEXINFO);
        addStructEquality(FMOD_CREATESOUNDEXINFO);
        addStructConstructor(FMOD_CREATESOUNDEXINFO, "CREATESOUNDEXINFO", -2);
        /* int cbsize */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, cbsize, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, cbsize, int);
        /* unsigned int length */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, length, unsigned_int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, length, unsigned_int);
        /* unsigned int fileoffset */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, fileoffset, unsigned_int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, fileoffset, unsigned_int);
        /* int numchannels */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, numchannels, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, numchannels, int);
        /* int defaultfrequency */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, defaultfrequency, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, defaultfrequency, int);
        /* FMOD_SOUND_FORMAT format */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, format, FMOD_SOUND_FORMAT);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, format, FMOD_SOUND_FORMAT);
        /* unsigned int decodebuffersize */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, decodebuffersize, unsigned_int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, decodebuffersize, unsigned_int);
        /* int initialsubsound */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, initialsubsound, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, initialsubsound, int);
        /* int numsubsounds */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, numsubsounds, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, numsubsounds, int);
        /* int* inclusionlist */
        /* int inclusionlistnum */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, inclusionlistnum, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, inclusionlistnum, int);
        /* FMOD_SOUND_PCMREAD_CALLBACK pcmreadcallback */
        /* FMOD_SOUND_PCMSETPOS_CALLBACK pcmsetposcallback */
        /* FMOD_SOUND_NONBLOCK_CALLBACK nonblockcallback */
        /* const char* dlsname */
        /* const char* encryptionkey */
        /* int maxpolyphony */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, maxpolyphony, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, maxpolyphony, int);
        /* void* userdata */
        /* FMOD_SOUND_TYPE suggestedsoundtype */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, suggestedsoundtype, FMOD_SOUND_TYPE);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, suggestedsoundtype, FMOD_SOUND_TYPE);
        /* FMOD_FILE_OPEN_CALLBACK fileuseropen */
        /* FMOD_FILE_CLOSE_CALLBACK fileuserclose */
        /* FMOD_FILE_READ_CALLBACK fileuserread */
        /* FMOD_FILE_SEEK_CALLBACK fileuserseek */
        /* FMOD_FILE_ASYNCREAD_CALLBACK fileuserasyncread */
        /* FMOD_FILE_ASYNCCANCEL_CALLBACK fileuserasynccancel */
        /* void* fileuserdata */
        /* int filebuffersize */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, filebuffersize, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, filebuffersize, int);
        /* FMOD_CHANNELORDER channelorder */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, channelorder, FMOD_CHANNELORDER);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, channelorder, FMOD_CHANNELORDER);
        /* FMOD_SOUNDGROUP* initialsoundgroup */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, initialsoundgroup, ptr_FMOD_SOUNDGROUP);
        /* unsigned int initialseekposition */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, initialseekposition, unsigned_int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, initialseekposition, unsigned_int);
        /* FMOD_TIMEUNIT initialseekpostype */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, initialseekpostype, unsigned_int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, initialseekpostype, unsigned_int);
        /* int ignoresetfilesystem */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, ignoresetfilesystem, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, ignoresetfilesystem, int);
        /* unsigned int audioqueuepolicy */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, audioqueuepolicy, unsigned_int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, audioqueuepolicy, unsigned_int);
        /* unsigned int minmidigranularity */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, minmidigranularity, unsigned_int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, minmidigranularity, unsigned_int);
        /* int nonblockthreadid */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, nonblockthreadid, int);
        addPropertySetter(FMOD_CREATESOUNDEXINFO, nonblockthreadid, int);
        /* FMOD_GUID* fsbguid */
        addPropertyGetter(FMOD_CREATESOUNDEXINFO, fsbguid, ptr_FMOD_GUID);
        #ifdef FMODExt_extras_FMOD_CREATESOUNDEXINFO
        FMODExt_extras_FMOD_CREATESOUNDEXINFO
        #endif
    endStruct();
    beginStruct(FMOD_REVERB_PROPERTIES);
        addStructEquality(FMOD_REVERB_PROPERTIES);
        addStructConstructor(FMOD_REVERB_PROPERTIES, "REVERB_PROPERTIES", -2);
        /* float DecayTime */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, DecayTime, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, DecayTime, float);
        /* float EarlyDelay */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, EarlyDelay, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, EarlyDelay, float);
        /* float LateDelay */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, LateDelay, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, LateDelay, float);
        /* float HFReference */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, HFReference, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, HFReference, float);
        /* float HFDecayRatio */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, HFDecayRatio, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, HFDecayRatio, float);
        /* float Diffusion */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, Diffusion, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, Diffusion, float);
        /* float Density */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, Density, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, Density, float);
        /* float LowShelfFrequency */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, LowShelfFrequency, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, LowShelfFrequency, float);
        /* float LowShelfGain */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, LowShelfGain, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, LowShelfGain, float);
        /* float HighCut */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, HighCut, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, HighCut, float);
        /* float EarlyLateMix */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, EarlyLateMix, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, EarlyLateMix, float);
        /* float WetLevel */
        addPropertyGetter(FMOD_REVERB_PROPERTIES, WetLevel, float);
        addPropertySetter(FMOD_REVERB_PROPERTIES, WetLevel, float);
        #ifdef FMODExt_extras_FMOD_REVERB_PROPERTIES
        FMODExt_extras_FMOD_REVERB_PROPERTIES
        #endif
    endStruct();
    beginStruct(FMOD_ERRORCALLBACK_INFO);
        addStructEquality(FMOD_ERRORCALLBACK_INFO);
        addStructConstructor(FMOD_ERRORCALLBACK_INFO, "ERRORCALLBACK_INFO", -2);
        /* FMOD_RESULT result */
        addPropertyGetter(FMOD_ERRORCALLBACK_INFO, result, FMOD_RESULT);
        addPropertySetter(FMOD_ERRORCALLBACK_INFO, result, FMOD_RESULT);
        /* FMOD_ERRORCALLBACK_INSTANCETYPE instancetype */
        addPropertyGetter(FMOD_ERRORCALLBACK_INFO, instancetype, FMOD_ERRORCALLBACK_INSTANCETYPE);
        addPropertySetter(FMOD_ERRORCALLBACK_INFO, instancetype, FMOD_ERRORCALLBACK_INSTANCETYPE);
        /* void* instance */
        /* const char* functionname */
        /* const char* functionparams */
        #ifdef FMODExt_extras_FMOD_ERRORCALLBACK_INFO
        FMODExt_extras_FMOD_ERRORCALLBACK_INFO
        #endif
    endStruct();
    beginStruct(FMOD_CPU_USAGE);
        addStructEquality(FMOD_CPU_USAGE);
        addStructConstructor(FMOD_CPU_USAGE, "CPU_USAGE", -2);
        /* float dsp */
        addPropertyGetter(FMOD_CPU_USAGE, dsp, float);
        addPropertySetter(FMOD_CPU_USAGE, dsp, float);
        /* float stream */
        addPropertyGetter(FMOD_CPU_USAGE, stream, float);
        addPropertySetter(FMOD_CPU_USAGE, stream, float);
        /* float geometry */
        addPropertyGetter(FMOD_CPU_USAGE, geometry, float);
        addPropertySetter(FMOD_CPU_USAGE, geometry, float);
        /* float update */
        addPropertyGetter(FMOD_CPU_USAGE, update, float);
        addPropertySetter(FMOD_CPU_USAGE, update, float);
        /* float convolution1 */
        addPropertyGetter(FMOD_CPU_USAGE, convolution1, float);
        addPropertySetter(FMOD_CPU_USAGE, convolution1, float);
        /* float convolution2 */
        addPropertyGetter(FMOD_CPU_USAGE, convolution2, float);
        addPropertySetter(FMOD_CPU_USAGE, convolution2, float);
        #ifdef FMODExt_extras_FMOD_CPU_USAGE
        FMODExt_extras_FMOD_CPU_USAGE
        #endif
    endStruct();
    beginStruct(FMOD_DSP_DATA_PARAMETER_INFO);
        addStructEquality(FMOD_DSP_DATA_PARAMETER_INFO);
        addStructConstructor(FMOD_DSP_DATA_PARAMETER_INFO, "DSP_DATA_PARAMETER_INFO", -2);
        /* void* data */
        /* unsigned int length */
        addPropertyGetter(FMOD_DSP_DATA_PARAMETER_INFO, length, unsigned_int);
        addPropertySetter(FMOD_DSP_DATA_PARAMETER_INFO, length, unsigned_int);
        /* int index */
        addPropertyGetter(FMOD_DSP_DATA_PARAMETER_INFO, index, int);
        addPropertySetter(FMOD_DSP_DATA_PARAMETER_INFO, index, int);
        #ifdef FMODExt_extras_FMOD_DSP_DATA_PARAMETER_INFO
        FMODExt_extras_FMOD_DSP_DATA_PARAMETER_INFO
        #endif
    endStruct();
    beginStruct(FMOD_CODEC_STATE);
        addStructEquality(FMOD_CODEC_STATE);
        addStructConstructor(FMOD_CODEC_STATE, "CODEC_STATE", -2);
        /* void* plugindata */
        /* FMOD_CODEC_WAVEFORMAT* waveformat */
        addPropertyGetter(FMOD_CODEC_STATE, waveformat, ptr_FMOD_CODEC_WAVEFORMAT);
        /* FMOD_CODEC_STATE_FUNCTIONS* functions */
        addPropertyGetter(FMOD_CODEC_STATE, functions, ptr_FMOD_CODEC_STATE_FUNCTIONS);
        /* int numsubsounds */
        addPropertyGetter(FMOD_CODEC_STATE, numsubsounds, int);
        addPropertySetter(FMOD_CODEC_STATE, numsubsounds, int);
        #ifdef FMODExt_extras_FMOD_CODEC_STATE
        FMODExt_extras_FMOD_CODEC_STATE
        #endif
    endStruct();
    beginStruct(FMOD_CODEC_WAVEFORMAT);
        addStructEquality(FMOD_CODEC_WAVEFORMAT);
        addStructConstructor(FMOD_CODEC_WAVEFORMAT, "CODEC_WAVEFORMAT", -2);
        /* const char* name */
        /* FMOD_SOUND_FORMAT format */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, format, FMOD_SOUND_FORMAT);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, format, FMOD_SOUND_FORMAT);
        /* int channels */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, channels, int);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, channels, int);
        /* int frequency */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, frequency, int);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, frequency, int);
        /* unsigned int lengthbytes */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, lengthbytes, unsigned_int);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, lengthbytes, unsigned_int);
        /* unsigned int lengthpcm */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, lengthpcm, unsigned_int);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, lengthpcm, unsigned_int);
        /* unsigned int pcmblocksize */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, pcmblocksize, unsigned_int);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, pcmblocksize, unsigned_int);
        /* int loopstart */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, loopstart, int);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, loopstart, int);
        /* int loopend */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, loopend, int);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, loopend, int);
        /* FMOD_MODE mode */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, mode, unsigned_int);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, mode, unsigned_int);
        /* FMOD_CHANNELMASK channelmask */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, channelmask, unsigned_int);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, channelmask, unsigned_int);
        /* FMOD_CHANNELORDER channelorder */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, channelorder, FMOD_CHANNELORDER);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, channelorder, FMOD_CHANNELORDER);
        /* float peakvolume */
        addPropertyGetter(FMOD_CODEC_WAVEFORMAT, peakvolume, float);
        addPropertySetter(FMOD_CODEC_WAVEFORMAT, peakvolume, float);
        #ifdef FMODExt_extras_FMOD_CODEC_WAVEFORMAT
        FMODExt_extras_FMOD_CODEC_WAVEFORMAT
        #endif
    endStruct();
    beginStruct(FMOD_CODEC_DESCRIPTION);
        addStructEquality(FMOD_CODEC_DESCRIPTION);
        addStructConstructor(FMOD_CODEC_DESCRIPTION, "CODEC_DESCRIPTION", -2);
        /* unsigned int apiversion */
        addPropertyGetter(FMOD_CODEC_DESCRIPTION, apiversion, unsigned_int);
        addPropertySetter(FMOD_CODEC_DESCRIPTION, apiversion, unsigned_int);
        /* const char* name */
        /* unsigned int version */
        addPropertyGetter(FMOD_CODEC_DESCRIPTION, version, unsigned_int);
        addPropertySetter(FMOD_CODEC_DESCRIPTION, version, unsigned_int);
        /* int defaultasstream */
        addPropertyGetter(FMOD_CODEC_DESCRIPTION, defaultasstream, int);
        addPropertySetter(FMOD_CODEC_DESCRIPTION, defaultasstream, int);
        /* FMOD_TIMEUNIT timeunits */
        addPropertyGetter(FMOD_CODEC_DESCRIPTION, timeunits, unsigned_int);
        addPropertySetter(FMOD_CODEC_DESCRIPTION, timeunits, unsigned_int);
        /* FMOD_CODEC_OPEN_CALLBACK open */
        /* FMOD_CODEC_CLOSE_CALLBACK close */
        /* FMOD_CODEC_READ_CALLBACK read */
        /* FMOD_CODEC_GETLENGTH_CALLBACK getlength */
        /* FMOD_CODEC_SETPOSITION_CALLBACK setposition */
        /* FMOD_CODEC_GETPOSITION_CALLBACK getposition */
        /* FMOD_CODEC_SOUNDCREATE_CALLBACK soundcreate */
        /* FMOD_CODEC_GETWAVEFORMAT_CALLBACK getwaveformat */
        #ifdef FMODExt_extras_FMOD_CODEC_DESCRIPTION
        FMODExt_extras_FMOD_CODEC_DESCRIPTION
        #endif
    endStruct();
    beginStruct(FMOD_CODEC_STATE_FUNCTIONS);
        addStructEquality(FMOD_CODEC_STATE_FUNCTIONS);
        addStructConstructor(FMOD_CODEC_STATE_FUNCTIONS, "CODEC_STATE_FUNCTIONS", -2);
        /* FMOD_CODEC_METADATA_FUNC metadata */
        /* FMOD_CODEC_ALLOC_FUNC alloc */
        /* FMOD_CODEC_FREE_FUNC free */
        /* FMOD_CODEC_LOG_FUNC log */
        /* FMOD_CODEC_FILE_READ_FUNC read */
        /* FMOD_CODEC_FILE_SEEK_FUNC seek */
        /* FMOD_CODEC_FILE_TELL_FUNC tell */
        /* FMOD_CODEC_FILE_SIZE_FUNC size */
        #ifdef FMODExt_extras_FMOD_CODEC_STATE_FUNCTIONS
        FMODExt_extras_FMOD_CODEC_STATE_FUNCTIONS
        #endif
    endStruct();
    beginStruct(FMOD_DSP_LOUDNESS_METER_INFO_TYPE);
        addStructEquality(FMOD_DSP_LOUDNESS_METER_INFO_TYPE);
        addStructConstructor(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, "DSP_LOUDNESS_METER_INFO_TYPE", -2);
        /* float momentaryloudness */
        addPropertyGetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, momentaryloudness, float);
        addPropertySetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, momentaryloudness, float);
        /* float shorttermloudness */
        addPropertyGetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, shorttermloudness, float);
        addPropertySetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, shorttermloudness, float);
        /* float integratedloudness */
        addPropertyGetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, integratedloudness, float);
        addPropertySetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, integratedloudness, float);
        /* float loudness10thpercentile */
        addPropertyGetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, loudness10thpercentile, float);
        addPropertySetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, loudness10thpercentile, float);
        /* float loudness95thpercentile */
        addPropertyGetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, loudness95thpercentile, float);
        addPropertySetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, loudness95thpercentile, float);
        /* __UNKNOWN__ loudnesshistogram */
        /* float maxtruepeak */
        addPropertyGetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, maxtruepeak, float);
        addPropertySetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, maxtruepeak, float);
        /* float maxmomentaryloudness */
        addPropertyGetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, maxmomentaryloudness, float);
        addPropertySetter(FMOD_DSP_LOUDNESS_METER_INFO_TYPE, maxmomentaryloudness, float);
        #ifdef FMODExt_extras_FMOD_DSP_LOUDNESS_METER_INFO_TYPE
        FMODExt_extras_FMOD_DSP_LOUDNESS_METER_INFO_TYPE
        #endif
    endStruct();
    beginStruct(FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE);
        addStructEquality(FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE);
        addStructConstructor(FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE, "DSP_LOUDNESS_METER_WEIGHTING_TYPE", -2);
        /* __UNKNOWN__ channelweight */
        #ifdef FMODExt_extras_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE
        FMODExt_extras_FMOD_DSP_LOUDNESS_METER_WEIGHTING_TYPE
        #endif
    endStruct();
    beginStruct(FMOD_DSP_STATE);
        addStructEquality(FMOD_DSP_STATE);
        addStructConstructor(FMOD_DSP_STATE, "DSP_STATE", -2);
        /* void* instance */
        /* void* plugindata */
        /* FMOD_CHANNELMASK channelmask */
        addPropertyGetter(FMOD_DSP_STATE, channelmask, unsigned_int);
        addPropertySetter(FMOD_DSP_STATE, channelmask, unsigned_int);
        /* FMOD_SPEAKERMODE source_speakermode */
        addPropertyGetter(FMOD_DSP_STATE, source_speakermode, FMOD_SPEAKERMODE);
        addPropertySetter(FMOD_DSP_STATE, source_speakermode, FMOD_SPEAKERMODE);
        /* float* sidechaindata */
        /* int sidechainchannels */
        addPropertyGetter(FMOD_DSP_STATE, sidechainchannels, int);
        addPropertySetter(FMOD_DSP_STATE, sidechainchannels, int);
        /* FMOD_DSP_STATE_FUNCTIONS* functions */
        addPropertyGetter(FMOD_DSP_STATE, functions, ptr_FMOD_DSP_STATE_FUNCTIONS);
        /* int systemobject */
        addPropertyGetter(FMOD_DSP_STATE, systemobject, int);
        addPropertySetter(FMOD_DSP_STATE, systemobject, int);
        #ifdef FMODExt_extras_FMOD_DSP_STATE
        FMODExt_extras_FMOD_DSP_STATE
        #endif
    endStruct();
    beginStruct(FMOD_DSP_BUFFER_ARRAY);
        addStructEquality(FMOD_DSP_BUFFER_ARRAY);
        addStructConstructor(FMOD_DSP_BUFFER_ARRAY, "DSP_BUFFER_ARRAY", -2);
        /* int numbuffers */
        addPropertyGetter(FMOD_DSP_BUFFER_ARRAY, numbuffers, int);
        addPropertySetter(FMOD_DSP_BUFFER_ARRAY, numbuffers, int);
        /* int* buffernumchannels */
        /* FMOD_CHANNELMASK* bufferchannelmask */
        /* float** buffers */
        /* FMOD_SPEAKERMODE speakermode */
        addPropertyGetter(FMOD_DSP_BUFFER_ARRAY, speakermode, FMOD_SPEAKERMODE);
        addPropertySetter(FMOD_DSP_BUFFER_ARRAY, speakermode, FMOD_SPEAKERMODE);
        #ifdef FMODExt_extras_FMOD_DSP_BUFFER_ARRAY
        FMODExt_extras_FMOD_DSP_BUFFER_ARRAY
        #endif
    endStruct();
    beginStruct(FMOD_COMPLEX);
        addStructEquality(FMOD_COMPLEX);
        addStructConstructor(FMOD_COMPLEX, "COMPLEX", -2);
        /* float real */
        addPropertyGetter(FMOD_COMPLEX, real, float);
        addPropertySetter(FMOD_COMPLEX, real, float);
        /* float imag */
        addPropertyGetter(FMOD_COMPLEX, imag, float);
        addPropertySetter(FMOD_COMPLEX, imag, float);
        #ifdef FMODExt_extras_FMOD_COMPLEX
        FMODExt_extras_FMOD_COMPLEX
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR);
        addStructEquality(FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR);
        addStructConstructor(FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR, "DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR", -2);
        /* int numpoints */
        addPropertyGetter(FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR, numpoints, int);
        addPropertySetter(FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR, numpoints, int);
        /* float* pointparamvalues */
        /* float* pointpositions */
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR
        FMODExt_extras_FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_FLOAT_MAPPING);
        addStructEquality(FMOD_DSP_PARAMETER_FLOAT_MAPPING);
        addStructConstructor(FMOD_DSP_PARAMETER_FLOAT_MAPPING, "DSP_PARAMETER_FLOAT_MAPPING", -2);
        /* FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE type */
        addPropertyGetter(FMOD_DSP_PARAMETER_FLOAT_MAPPING, type, FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE);
        addPropertySetter(FMOD_DSP_PARAMETER_FLOAT_MAPPING, type, FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE);
        /* FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR piecewiselinearmapping */
        addPropertyGetter(FMOD_DSP_PARAMETER_FLOAT_MAPPING, piecewiselinearmapping, FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR);
        addPropertySetter(FMOD_DSP_PARAMETER_FLOAT_MAPPING, piecewiselinearmapping, FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR);
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_FLOAT_MAPPING
        FMODExt_extras_FMOD_DSP_PARAMETER_FLOAT_MAPPING
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_DESC_FLOAT);
        addStructEquality(FMOD_DSP_PARAMETER_DESC_FLOAT);
        addStructConstructor(FMOD_DSP_PARAMETER_DESC_FLOAT, "DSP_PARAMETER_DESC_FLOAT", -2);
        /* float min */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_FLOAT, min, float);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_FLOAT, min, float);
        /* float max */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_FLOAT, max, float);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_FLOAT, max, float);
        /* float defaultval */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_FLOAT, defaultval, float);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_FLOAT, defaultval, float);
        /* FMOD_DSP_PARAMETER_FLOAT_MAPPING mapping */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_FLOAT, mapping, FMOD_DSP_PARAMETER_FLOAT_MAPPING);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_FLOAT, mapping, FMOD_DSP_PARAMETER_FLOAT_MAPPING);
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_DESC_FLOAT
        FMODExt_extras_FMOD_DSP_PARAMETER_DESC_FLOAT
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_DESC_INT);
        addStructEquality(FMOD_DSP_PARAMETER_DESC_INT);
        addStructConstructor(FMOD_DSP_PARAMETER_DESC_INT, "DSP_PARAMETER_DESC_INT", -2);
        /* int min */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_INT, min, int);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_INT, min, int);
        /* int max */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_INT, max, int);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_INT, max, int);
        /* int defaultval */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_INT, defaultval, int);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_INT, defaultval, int);
        /* FMOD_BOOL goestoinf */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_INT, goestoinf, FMOD_BOOL);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_INT, goestoinf, FMOD_BOOL);
        /* const char* const* valuenames */
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_DESC_INT
        FMODExt_extras_FMOD_DSP_PARAMETER_DESC_INT
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_DESC_BOOL);
        addStructEquality(FMOD_DSP_PARAMETER_DESC_BOOL);
        addStructConstructor(FMOD_DSP_PARAMETER_DESC_BOOL, "DSP_PARAMETER_DESC_BOOL", -2);
        /* FMOD_BOOL defaultval */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_BOOL, defaultval, FMOD_BOOL);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_BOOL, defaultval, FMOD_BOOL);
        /* const char* const* valuenames */
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_DESC_BOOL
        FMODExt_extras_FMOD_DSP_PARAMETER_DESC_BOOL
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_DESC_DATA);
        addStructEquality(FMOD_DSP_PARAMETER_DESC_DATA);
        addStructConstructor(FMOD_DSP_PARAMETER_DESC_DATA, "DSP_PARAMETER_DESC_DATA", -2);
        /* int datatype */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC_DATA, datatype, int);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC_DATA, datatype, int);
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_DESC_DATA
        FMODExt_extras_FMOD_DSP_PARAMETER_DESC_DATA
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_DESC);
        addStructEquality(FMOD_DSP_PARAMETER_DESC);
        addStructConstructor(FMOD_DSP_PARAMETER_DESC, "DSP_PARAMETER_DESC", -2);
        /* FMOD_DSP_PARAMETER_TYPE type */
        addPropertyGetter(FMOD_DSP_PARAMETER_DESC, type, FMOD_DSP_PARAMETER_TYPE);
        addPropertySetter(FMOD_DSP_PARAMETER_DESC, type, FMOD_DSP_PARAMETER_TYPE);
        /* __UNKNOWN__ name */
        /* __UNKNOWN__ label */
        /* const char* description */
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_DESC
        FMODExt_extras_FMOD_DSP_PARAMETER_DESC
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_OVERALLGAIN);
        addStructEquality(FMOD_DSP_PARAMETER_OVERALLGAIN);
        addStructConstructor(FMOD_DSP_PARAMETER_OVERALLGAIN, "DSP_PARAMETER_OVERALLGAIN", -2);
        /* float linear_gain */
        addPropertyGetter(FMOD_DSP_PARAMETER_OVERALLGAIN, linear_gain, float);
        addPropertySetter(FMOD_DSP_PARAMETER_OVERALLGAIN, linear_gain, float);
        /* float linear_gain_additive */
        addPropertyGetter(FMOD_DSP_PARAMETER_OVERALLGAIN, linear_gain_additive, float);
        addPropertySetter(FMOD_DSP_PARAMETER_OVERALLGAIN, linear_gain_additive, float);
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_OVERALLGAIN
        FMODExt_extras_FMOD_DSP_PARAMETER_OVERALLGAIN
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_3DATTRIBUTES);
        addStructEquality(FMOD_DSP_PARAMETER_3DATTRIBUTES);
        addStructConstructor(FMOD_DSP_PARAMETER_3DATTRIBUTES, "DSP_PARAMETER_3DATTRIBUTES", -2);
        /* FMOD_3D_ATTRIBUTES relative */
        addPropertyGetter(FMOD_DSP_PARAMETER_3DATTRIBUTES, relative, FMOD_3D_ATTRIBUTES);
        addPropertySetter(FMOD_DSP_PARAMETER_3DATTRIBUTES, relative, FMOD_3D_ATTRIBUTES);
        /* FMOD_3D_ATTRIBUTES absolute */
        addPropertyGetter(FMOD_DSP_PARAMETER_3DATTRIBUTES, absolute, FMOD_3D_ATTRIBUTES);
        addPropertySetter(FMOD_DSP_PARAMETER_3DATTRIBUTES, absolute, FMOD_3D_ATTRIBUTES);
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_3DATTRIBUTES
        FMODExt_extras_FMOD_DSP_PARAMETER_3DATTRIBUTES
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI);
        addStructEquality(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI);
        addStructConstructor(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI, "DSP_PARAMETER_3DATTRIBUTES_MULTI", -2);
        /* int numlisteners */
        addPropertyGetter(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI, numlisteners, int);
        addPropertySetter(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI, numlisteners, int);
        /* __UNKNOWN__ relative */
        /* __UNKNOWN__ weight */
        /* FMOD_3D_ATTRIBUTES absolute */
        addPropertyGetter(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI, absolute, FMOD_3D_ATTRIBUTES);
        addPropertySetter(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI, absolute, FMOD_3D_ATTRIBUTES);
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI
        FMODExt_extras_FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_ATTENUATION_RANGE);
        addStructEquality(FMOD_DSP_PARAMETER_ATTENUATION_RANGE);
        addStructConstructor(FMOD_DSP_PARAMETER_ATTENUATION_RANGE, "DSP_PARAMETER_ATTENUATION_RANGE", -2);
        /* float min */
        addPropertyGetter(FMOD_DSP_PARAMETER_ATTENUATION_RANGE, min, float);
        addPropertySetter(FMOD_DSP_PARAMETER_ATTENUATION_RANGE, min, float);
        /* float max */
        addPropertyGetter(FMOD_DSP_PARAMETER_ATTENUATION_RANGE, max, float);
        addPropertySetter(FMOD_DSP_PARAMETER_ATTENUATION_RANGE, max, float);
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_ATTENUATION_RANGE
        FMODExt_extras_FMOD_DSP_PARAMETER_ATTENUATION_RANGE
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_SIDECHAIN);
        addStructEquality(FMOD_DSP_PARAMETER_SIDECHAIN);
        addStructConstructor(FMOD_DSP_PARAMETER_SIDECHAIN, "DSP_PARAMETER_SIDECHAIN", -2);
        /* FMOD_BOOL sidechainenable */
        addPropertyGetter(FMOD_DSP_PARAMETER_SIDECHAIN, sidechainenable, FMOD_BOOL);
        addPropertySetter(FMOD_DSP_PARAMETER_SIDECHAIN, sidechainenable, FMOD_BOOL);
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_SIDECHAIN
        FMODExt_extras_FMOD_DSP_PARAMETER_SIDECHAIN
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_FFT);
        addStructEquality(FMOD_DSP_PARAMETER_FFT);
        addStructConstructor(FMOD_DSP_PARAMETER_FFT, "DSP_PARAMETER_FFT", -2);
        /* int length */
        addPropertyGetter(FMOD_DSP_PARAMETER_FFT, length, int);
        addPropertySetter(FMOD_DSP_PARAMETER_FFT, length, int);
        /* int numchannels */
        addPropertyGetter(FMOD_DSP_PARAMETER_FFT, numchannels, int);
        addPropertySetter(FMOD_DSP_PARAMETER_FFT, numchannels, int);
        /* __UNKNOWN__ spectrum */
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_FFT
        FMODExt_extras_FMOD_DSP_PARAMETER_FFT
        #endif
    endStruct();
    beginStruct(FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE);
        addStructEquality(FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE);
        addStructConstructor(FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE, "DSP_PARAMETER_DYNAMIC_RESPONSE", -2);
        /* int numchannels */
        addPropertyGetter(FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE, numchannels, int);
        addPropertySetter(FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE, numchannels, int);
        /* __UNKNOWN__ rms */
        #ifdef FMODExt_extras_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE
        FMODExt_extras_FMOD_DSP_PARAMETER_DYNAMIC_RESPONSE
        #endif
    endStruct();
    beginStruct(FMOD_DSP_DESCRIPTION);
        addStructEquality(FMOD_DSP_DESCRIPTION);
        addStructConstructor(FMOD_DSP_DESCRIPTION, "DSP_DESCRIPTION", -2);
        /* unsigned int pluginsdkversion */
        addPropertyGetter(FMOD_DSP_DESCRIPTION, pluginsdkversion, unsigned_int);
        addPropertySetter(FMOD_DSP_DESCRIPTION, pluginsdkversion, unsigned_int);
        /* __UNKNOWN__ name */
        /* unsigned int version */
        addPropertyGetter(FMOD_DSP_DESCRIPTION, version, unsigned_int);
        addPropertySetter(FMOD_DSP_DESCRIPTION, version, unsigned_int);
        /* int numinputbuffers */
        addPropertyGetter(FMOD_DSP_DESCRIPTION, numinputbuffers, int);
        addPropertySetter(FMOD_DSP_DESCRIPTION, numinputbuffers, int);
        /* int numoutputbuffers */
        addPropertyGetter(FMOD_DSP_DESCRIPTION, numoutputbuffers, int);
        addPropertySetter(FMOD_DSP_DESCRIPTION, numoutputbuffers, int);
        /* FMOD_DSP_CREATE_CALLBACK create */
        /* FMOD_DSP_RELEASE_CALLBACK release */
        /* FMOD_DSP_RESET_CALLBACK reset */
        /* FMOD_DSP_READ_CALLBACK read */
        /* FMOD_DSP_PROCESS_CALLBACK process */
        /* FMOD_DSP_SETPOSITION_CALLBACK setposition */
        /* int numparameters */
        addPropertyGetter(FMOD_DSP_DESCRIPTION, numparameters, int);
        addPropertySetter(FMOD_DSP_DESCRIPTION, numparameters, int);
        /* FMOD_DSP_PARAMETER_DESC** paramdesc */
        /* FMOD_DSP_SETPARAM_FLOAT_CALLBACK setparameterfloat */
        /* FMOD_DSP_SETPARAM_INT_CALLBACK setparameterint */
        /* FMOD_DSP_SETPARAM_BOOL_CALLBACK setparameterbool */
        /* FMOD_DSP_SETPARAM_DATA_CALLBACK setparameterdata */
        /* FMOD_DSP_GETPARAM_FLOAT_CALLBACK getparameterfloat */
        /* FMOD_DSP_GETPARAM_INT_CALLBACK getparameterint */
        /* FMOD_DSP_GETPARAM_BOOL_CALLBACK getparameterbool */
        /* FMOD_DSP_GETPARAM_DATA_CALLBACK getparameterdata */
        /* FMOD_DSP_SHOULDIPROCESS_CALLBACK shouldiprocess */
        /* void* userdata */
        /* FMOD_DSP_SYSTEM_REGISTER_CALLBACK sys_register */
        /* FMOD_DSP_SYSTEM_DEREGISTER_CALLBACK sys_deregister */
        /* FMOD_DSP_SYSTEM_MIX_CALLBACK sys_mix */
        #ifdef FMODExt_extras_FMOD_DSP_DESCRIPTION
        FMODExt_extras_FMOD_DSP_DESCRIPTION
        #endif
    endStruct();
    beginStruct(FMOD_DSP_STATE_DFT_FUNCTIONS);
        addStructEquality(FMOD_DSP_STATE_DFT_FUNCTIONS);
        addStructConstructor(FMOD_DSP_STATE_DFT_FUNCTIONS, "DSP_STATE_DFT_FUNCTIONS", -2);
        /* FMOD_DSP_DFT_FFTREAL_FUNC fftreal */
        /* FMOD_DSP_DFT_IFFTREAL_FUNC inversefftreal */
        #ifdef FMODExt_extras_FMOD_DSP_STATE_DFT_FUNCTIONS
        FMODExt_extras_FMOD_DSP_STATE_DFT_FUNCTIONS
        #endif
    endStruct();
    beginStruct(FMOD_DSP_STATE_PAN_FUNCTIONS);
        addStructEquality(FMOD_DSP_STATE_PAN_FUNCTIONS);
        addStructConstructor(FMOD_DSP_STATE_PAN_FUNCTIONS, "DSP_STATE_PAN_FUNCTIONS", -2);
        /* FMOD_DSP_PAN_SUMMONOMATRIX_FUNC summonomatrix */
        /* FMOD_DSP_PAN_SUMSTEREOMATRIX_FUNC sumstereomatrix */
        /* FMOD_DSP_PAN_SUMSURROUNDMATRIX_FUNC sumsurroundmatrix */
        /* FMOD_DSP_PAN_SUMMONOTOSURROUNDMATRIX_FUNC summonotosurroundmatrix */
        /* FMOD_DSP_PAN_SUMSTEREOTOSURROUNDMATRIX_FUNC sumstereotosurroundmatrix */
        /* FMOD_DSP_PAN_GETROLLOFFGAIN_FUNC getrolloffgain */
        #ifdef FMODExt_extras_FMOD_DSP_STATE_PAN_FUNCTIONS
        FMODExt_extras_FMOD_DSP_STATE_PAN_FUNCTIONS
        #endif
    endStruct();
    beginStruct(FMOD_DSP_STATE_FUNCTIONS);
        addStructEquality(FMOD_DSP_STATE_FUNCTIONS);
        addStructConstructor(FMOD_DSP_STATE_FUNCTIONS, "DSP_STATE_FUNCTIONS", -2);
        /* FMOD_DSP_ALLOC_FUNC alloc */
        /* FMOD_DSP_REALLOC_FUNC realloc */
        /* FMOD_DSP_FREE_FUNC free */
        /* FMOD_DSP_GETSAMPLERATE_FUNC getsamplerate */
        /* FMOD_DSP_GETBLOCKSIZE_FUNC getblocksize */
        /* FMOD_DSP_STATE_DFT_FUNCTIONS* dft */
        addPropertyGetter(FMOD_DSP_STATE_FUNCTIONS, dft, ptr_FMOD_DSP_STATE_DFT_FUNCTIONS);
        /* FMOD_DSP_STATE_PAN_FUNCTIONS* pan */
        addPropertyGetter(FMOD_DSP_STATE_FUNCTIONS, pan, ptr_FMOD_DSP_STATE_PAN_FUNCTIONS);
        /* FMOD_DSP_GETSPEAKERMODE_FUNC getspeakermode */
        /* FMOD_DSP_GETCLOCK_FUNC getclock */
        /* FMOD_DSP_GETLISTENERATTRIBUTES_FUNC getlistenerattributes */
        /* FMOD_DSP_LOG_FUNC log */
        /* FMOD_DSP_GETUSERDATA_FUNC getuserdata */
        #ifdef FMODExt_extras_FMOD_DSP_STATE_FUNCTIONS
        FMODExt_extras_FMOD_DSP_STATE_FUNCTIONS
        #endif
    endStruct();
    beginStruct(FMOD_DSP_METERING_INFO);
        addStructEquality(FMOD_DSP_METERING_INFO);
        addStructConstructor(FMOD_DSP_METERING_INFO, "DSP_METERING_INFO", -2);
        /* int numsamples */
        addPropertyGetter(FMOD_DSP_METERING_INFO, numsamples, int);
        addPropertySetter(FMOD_DSP_METERING_INFO, numsamples, int);
        /* __UNKNOWN__ peaklevel */
        /* __UNKNOWN__ rmslevel */
        /* short numchannels */
        addPropertyGetter(FMOD_DSP_METERING_INFO, numchannels, short);
        addPropertySetter(FMOD_DSP_METERING_INFO, numchannels, short);
        #ifdef FMODExt_extras_FMOD_DSP_METERING_INFO
        FMODExt_extras_FMOD_DSP_METERING_INFO
        #endif
    endStruct();
    beginStruct(FMOD_OUTPUT_STATE);
        addStructEquality(FMOD_OUTPUT_STATE);
        addStructConstructor(FMOD_OUTPUT_STATE, "OUTPUT_STATE", -2);
        /* void* plugindata */
        /* FMOD_OUTPUT_READFROMMIXER_FUNC readfrommixer */
        /* FMOD_OUTPUT_ALLOC_FUNC alloc */
        /* FMOD_OUTPUT_FREE_FUNC free */
        /* FMOD_OUTPUT_LOG_FUNC log */
        /* FMOD_OUTPUT_COPYPORT_FUNC copyport */
        /* FMOD_OUTPUT_REQUESTRESET_FUNC requestreset */
        #ifdef FMODExt_extras_FMOD_OUTPUT_STATE
        FMODExt_extras_FMOD_OUTPUT_STATE
        #endif
    endStruct();
    beginStruct(FMOD_OUTPUT_OBJECT3DINFO);
        addStructEquality(FMOD_OUTPUT_OBJECT3DINFO);
        addStructConstructor(FMOD_OUTPUT_OBJECT3DINFO, "OUTPUT_OBJECT3DINFO", -2);
        /* float* buffer */
        /* unsigned int bufferlength */
        addPropertyGetter(FMOD_OUTPUT_OBJECT3DINFO, bufferlength, unsigned_int);
        addPropertySetter(FMOD_OUTPUT_OBJECT3DINFO, bufferlength, unsigned_int);
        /* FMOD_VECTOR position */
        addPropertyGetter(FMOD_OUTPUT_OBJECT3DINFO, position, FMOD_VECTOR);
        addPropertySetter(FMOD_OUTPUT_OBJECT3DINFO, position, FMOD_VECTOR);
        /* float gain */
        addPropertyGetter(FMOD_OUTPUT_OBJECT3DINFO, gain, float);
        addPropertySetter(FMOD_OUTPUT_OBJECT3DINFO, gain, float);
        /* float spread */
        addPropertyGetter(FMOD_OUTPUT_OBJECT3DINFO, spread, float);
        addPropertySetter(FMOD_OUTPUT_OBJECT3DINFO, spread, float);
        /* float priority */
        addPropertyGetter(FMOD_OUTPUT_OBJECT3DINFO, priority, float);
        addPropertySetter(FMOD_OUTPUT_OBJECT3DINFO, priority, float);
        #ifdef FMODExt_extras_FMOD_OUTPUT_OBJECT3DINFO
        FMODExt_extras_FMOD_OUTPUT_OBJECT3DINFO
        #endif
    endStruct();
    beginStruct(FMOD_OUTPUT_DESCRIPTION);
        addStructEquality(FMOD_OUTPUT_DESCRIPTION);
        addStructConstructor(FMOD_OUTPUT_DESCRIPTION, "OUTPUT_DESCRIPTION", -2);
        /* unsigned int apiversion */
        addPropertyGetter(FMOD_OUTPUT_DESCRIPTION, apiversion, unsigned_int);
        addPropertySetter(FMOD_OUTPUT_DESCRIPTION, apiversion, unsigned_int);
        /* const char* name */
        /* unsigned int version */
        addPropertyGetter(FMOD_OUTPUT_DESCRIPTION, version, unsigned_int);
        addPropertySetter(FMOD_OUTPUT_DESCRIPTION, version, unsigned_int);
        /* FMOD_OUTPUT_METHOD method */
        addPropertyGetter(FMOD_OUTPUT_DESCRIPTION, method, unsigned_int);
        addPropertySetter(FMOD_OUTPUT_DESCRIPTION, method, unsigned_int);
        /* FMOD_OUTPUT_GETNUMDRIVERS_CALLBACK getnumdrivers */
        /* FMOD_OUTPUT_GETDRIVERINFO_CALLBACK getdriverinfo */
        /* FMOD_OUTPUT_INIT_CALLBACK init */
        /* FMOD_OUTPUT_START_CALLBACK start */
        /* FMOD_OUTPUT_STOP_CALLBACK stop */
        /* FMOD_OUTPUT_CLOSE_CALLBACK close */
        /* FMOD_OUTPUT_UPDATE_CALLBACK update */
        /* FMOD_OUTPUT_GETHANDLE_CALLBACK gethandle */
        /* FMOD_OUTPUT_MIXER_CALLBACK mixer */
        /* FMOD_OUTPUT_OBJECT3DGETINFO_CALLBACK object3dgetinfo */
        /* FMOD_OUTPUT_OBJECT3DALLOC_CALLBACK object3dalloc */
        /* FMOD_OUTPUT_OBJECT3DFREE_CALLBACK object3dfree */
        /* FMOD_OUTPUT_OBJECT3DUPDATE_CALLBACK object3dupdate */
        /* FMOD_OUTPUT_OPENPORT_CALLBACK openport */
        /* FMOD_OUTPUT_CLOSEPORT_CALLBACK closeport */
        /* FMOD_OUTPUT_DEVICELISTCHANGED_CALLBACK devicelistchanged */
        #ifdef FMODExt_extras_FMOD_OUTPUT_DESCRIPTION
        FMODExt_extras_FMOD_OUTPUT_DESCRIPTION
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_SYSTEM);
        addClassEquality(FMOD_STUDIO_SYSTEM);
        #ifdef FMODExt_func_FMOD_Studio_System_IsValid
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_IsValid);
        lua_setfield(L, -4, "is_valid");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetAdvancedSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetAdvancedSettings);
        lua_setfield(L, -4, "set_advanced_settings");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetAdvancedSettings
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetAdvancedSettings);
        lua_setfield(L, -4, "get_advanced_settings");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_Initialize
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_Initialize);
        lua_setfield(L, -4, "initialize");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_Update
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_Update);
        lua_setfield(L, -4, "update");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetCoreSystem
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetCoreSystem);
        lua_setfield(L, -4, "get_core_system");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetEvent
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetEvent);
        lua_setfield(L, -4, "get_event");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetBus
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetBus);
        lua_setfield(L, -4, "get_bus");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetVCA
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetVCA);
        lua_setfield(L, -4, "get_vca");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetBank
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetBank);
        lua_setfield(L, -4, "get_bank");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetEventByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetEventByID);
        lua_setfield(L, -4, "get_event_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetBusByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetBusByID);
        lua_setfield(L, -4, "get_bus_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetVCAByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetVCAByID);
        lua_setfield(L, -4, "get_vca_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetBankByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetBankByID);
        lua_setfield(L, -4, "get_bank_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetSoundInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetSoundInfo);
        lua_setfield(L, -4, "get_sound_info");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByName
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByName);
        lua_setfield(L, -4, "get_parameter_description_by_name");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetParameterDescriptionByID);
        lua_setfield(L, -4, "get_parameter_description_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetParameterLabelByName
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetParameterLabelByName);
        lua_setfield(L, -4, "get_parameter_label_by_name");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetParameterLabelByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetParameterLabelByID);
        lua_setfield(L, -4, "get_parameter_label_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetParameterByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetParameterByID);
        lua_setfield(L, -4, "get_parameter_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetParameterByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetParameterByID);
        lua_setfield(L, -4, "set_parameter_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetParameterByIDWithLabel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetParameterByIDWithLabel);
        lua_setfield(L, -4, "set_parameter_by_id_with_label");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetParametersByIDs
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetParametersByIDs);
        lua_setfield(L, -4, "set_parameters_by_ids");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetParameterByName
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetParameterByName);
        lua_setfield(L, -4, "get_parameter_by_name");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetParameterByName
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetParameterByName);
        lua_setfield(L, -4, "set_parameter_by_name");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetParameterByNameWithLabel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetParameterByNameWithLabel);
        lua_setfield(L, -4, "set_parameter_by_name_with_label");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_LookupID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_LookupID);
        lua_setfield(L, -4, "lookup_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_LookupPath
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_LookupPath);
        lua_setfield(L, -4, "lookup_path");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetNumListeners
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetNumListeners);
        lua_setfield(L, -4, "get_num_listeners");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetNumListeners
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetNumListeners);
        lua_setfield(L, -4, "set_num_listeners");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetListenerAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetListenerAttributes);
        lua_setfield(L, -4, "get_listener_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetListenerAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetListenerAttributes);
        lua_setfield(L, -4, "set_listener_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetListenerWeight
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetListenerWeight);
        lua_setfield(L, -4, "get_listener_weight");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetListenerWeight
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetListenerWeight);
        lua_setfield(L, -4, "set_listener_weight");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_LoadBankFile
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_LoadBankFile);
        lua_setfield(L, -4, "load_bank_file");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_LoadBankMemory
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_LoadBankMemory);
        lua_setfield(L, -4, "load_bank_memory");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_LoadBankCustom
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_LoadBankCustom);
        lua_setfield(L, -4, "load_bank_custom");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_RegisterPlugin
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_RegisterPlugin);
        lua_setfield(L, -4, "register_plugin");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_UnregisterPlugin
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_UnregisterPlugin);
        lua_setfield(L, -4, "unregister_plugin");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_UnloadAll
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_UnloadAll);
        lua_setfield(L, -4, "unload_all");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_FlushCommands
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_FlushCommands);
        lua_setfield(L, -4, "flush_commands");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_FlushSampleLoading
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_FlushSampleLoading);
        lua_setfield(L, -4, "flush_sample_loading");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_StartCommandCapture
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_StartCommandCapture);
        lua_setfield(L, -4, "start_command_capture");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_StopCommandCapture
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_StopCommandCapture);
        lua_setfield(L, -4, "stop_command_capture");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_LoadCommandReplay
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_LoadCommandReplay);
        lua_setfield(L, -4, "load_command_replay");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetBankCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetBankCount);
        lua_setfield(L, -4, "get_bank_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetBankList
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetBankList);
        lua_setfield(L, -4, "get_bank_list");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetParameterDescriptionCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetParameterDescriptionCount);
        lua_setfield(L, -4, "get_parameter_description_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetParameterDescriptionList
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetParameterDescriptionList);
        lua_setfield(L, -4, "get_parameter_description_list");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetCPUUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetCPUUsage);
        lua_setfield(L, -4, "get_cpu_usage");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetBufferUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetBufferUsage);
        lua_setfield(L, -4, "get_buffer_usage");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_ResetBufferUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_ResetBufferUsage);
        lua_setfield(L, -4, "reset_buffer_usage");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetCallback);
        lua_setfield(L, -4, "set_callback");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_System_GetMemoryUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_GetMemoryUsage);
        lua_setfield(L, -4, "get_memory_usage");
        #endif
        #ifdef FMODExt_extras_FMOD_STUDIO_SYSTEM
        FMODExt_extras_FMOD_STUDIO_SYSTEM
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_EVENTDESCRIPTION);
        addClassEquality(FMOD_STUDIO_EVENTDESCRIPTION);
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_IsValid
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_IsValid);
        lua_setfield(L, -4, "is_valid");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetID);
        lua_setfield(L, -4, "get_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetPath
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetPath);
        lua_setfield(L, -4, "get_path");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionCount);
        lua_setfield(L, -4, "get_parameter_description_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByIndex);
        lua_setfield(L, -4, "get_parameter_description_by_index");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByName
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByName);
        lua_setfield(L, -4, "get_parameter_description_by_name");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetParameterDescriptionByID);
        lua_setfield(L, -4, "get_parameter_description_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetParameterLabelByIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetParameterLabelByIndex);
        lua_setfield(L, -4, "get_parameter_label_by_index");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetParameterLabelByName
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetParameterLabelByName);
        lua_setfield(L, -4, "get_parameter_label_by_name");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetParameterLabelByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetParameterLabelByID);
        lua_setfield(L, -4, "get_parameter_label_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyCount);
        lua_setfield(L, -4, "get_user_property_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyByIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetUserPropertyByIndex);
        lua_setfield(L, -4, "get_user_property_by_index");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetUserProperty
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetUserProperty);
        lua_setfield(L, -4, "get_user_property");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetLength
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetLength);
        lua_setfield(L, -4, "get_length");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetMinMaxDistance
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetMinMaxDistance);
        lua_setfield(L, -4, "get_min_max_distance");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetSoundSize
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetSoundSize);
        lua_setfield(L, -4, "get_sound_size");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_IsSnapshot
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_IsSnapshot);
        lua_setfield(L, -4, "is_snapshot");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_IsOneshot
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_IsOneshot);
        lua_setfield(L, -4, "is_oneshot");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_IsStream
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_IsStream);
        lua_setfield(L, -4, "is_stream");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_Is3D
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_Is3D);
        lua_setfield(L, -4, "is_3d");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_IsDopplerEnabled
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_IsDopplerEnabled);
        lua_setfield(L, -4, "is_doppler_enabled");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_HasSustainPoint
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_HasSustainPoint);
        lua_setfield(L, -4, "has_sustain_point");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_CreateInstance
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_CreateInstance);
        lua_setfield(L, -4, "create_instance");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetInstanceCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetInstanceCount);
        lua_setfield(L, -4, "get_instance_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetInstanceList
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetInstanceList);
        lua_setfield(L, -4, "get_instance_list");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_LoadSampleData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_LoadSampleData);
        lua_setfield(L, -4, "load_sample_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_UnloadSampleData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_UnloadSampleData);
        lua_setfield(L, -4, "unload_sample_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetSampleLoadingState
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetSampleLoadingState);
        lua_setfield(L, -4, "get_sample_loading_state");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_ReleaseAllInstances
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_ReleaseAllInstances);
        lua_setfield(L, -4, "release_all_instances");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_SetCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_SetCallback);
        lua_setfield(L, -4, "set_callback");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventDescription_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventDescription_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_extras_FMOD_STUDIO_EVENTDESCRIPTION
        FMODExt_extras_FMOD_STUDIO_EVENTDESCRIPTION
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_EVENTINSTANCE);
        addClassEquality(FMOD_STUDIO_EVENTINSTANCE);
        addDestructor(FMOD_STUDIO_EVENTINSTANCE, FMOD_Studio_EventInstance_Release);
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_IsValid
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_IsValid);
        lua_setfield(L, -4, "is_valid");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetDescription
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetDescription);
        lua_setfield(L, -4, "get_description");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetSystem
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetSystem);
        lua_setfield(L, -4, "get_system");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetVolume);
        lua_setfield(L, -4, "get_volume");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetVolume);
        lua_setfield(L, -4, "set_volume");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetPitch
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetPitch);
        lua_setfield(L, -4, "get_pitch");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetPitch
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetPitch);
        lua_setfield(L, -4, "set_pitch");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_Get3DAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_Get3DAttributes);
        lua_setfield(L, -4, "get_3d_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_Set3DAttributes);
        lua_setfield(L, -4, "set_3d_attributes");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetListenerMask
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetListenerMask);
        lua_setfield(L, -4, "get_listener_mask");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetListenerMask
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetListenerMask);
        lua_setfield(L, -4, "set_listener_mask");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetProperty
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetProperty);
        lua_setfield(L, -4, "get_property");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetProperty
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetProperty);
        lua_setfield(L, -4, "set_property");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetReverbLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetReverbLevel);
        lua_setfield(L, -4, "get_reverb_level");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetReverbLevel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetReverbLevel);
        lua_setfield(L, -4, "set_reverb_level");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetPaused);
        lua_setfield(L, -4, "get_paused");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetPaused);
        lua_setfield(L, -4, "set_paused");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_Start
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_Start);
        lua_setfield(L, -4, "start");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_Stop
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_Stop);
        lua_setfield(L, -4, "stop");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetTimelinePosition
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetTimelinePosition);
        lua_setfield(L, -4, "get_timeline_position");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetTimelinePosition
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetTimelinePosition);
        lua_setfield(L, -4, "set_timeline_position");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetPlaybackState
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetPlaybackState);
        lua_setfield(L, -4, "get_playback_state");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetChannelGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetChannelGroup);
        lua_setfield(L, -4, "get_channel_group");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetMinMaxDistance
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetMinMaxDistance);
        lua_setfield(L, -4, "get_min_max_distance");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_IsVirtual
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_IsVirtual);
        lua_setfield(L, -4, "is_virtual");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetParameterByName
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetParameterByName);
        lua_setfield(L, -4, "get_parameter_by_name");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetParameterByName
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetParameterByName);
        lua_setfield(L, -4, "set_parameter_by_name");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetParameterByNameWithLabel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetParameterByNameWithLabel);
        lua_setfield(L, -4, "set_parameter_by_name_with_label");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetParameterByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetParameterByID);
        lua_setfield(L, -4, "get_parameter_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetParameterByID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetParameterByID);
        lua_setfield(L, -4, "set_parameter_by_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetParameterByIDWithLabel
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetParameterByIDWithLabel);
        lua_setfield(L, -4, "set_parameter_by_id_with_label");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetParametersByIDs
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetParametersByIDs);
        lua_setfield(L, -4, "set_parameters_by_ids");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_KeyOff
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_KeyOff);
        lua_setfield(L, -4, "key_off");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetCallback);
        lua_setfield(L, -4, "set_callback");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetCPUUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetCPUUsage);
        lua_setfield(L, -4, "get_cpu_usage");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_EventInstance_GetMemoryUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_EventInstance_GetMemoryUsage);
        lua_setfield(L, -4, "get_memory_usage");
        #endif
        #ifdef FMODExt_extras_FMOD_STUDIO_EVENTINSTANCE
        FMODExt_extras_FMOD_STUDIO_EVENTINSTANCE
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_BUS);
        addClassEquality(FMOD_STUDIO_BUS);
        #ifdef FMODExt_func_FMOD_Studio_Bus_IsValid
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_IsValid);
        lua_setfield(L, -4, "is_valid");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_GetID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_GetID);
        lua_setfield(L, -4, "get_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_GetPath
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_GetPath);
        lua_setfield(L, -4, "get_path");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_GetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_GetVolume);
        lua_setfield(L, -4, "get_volume");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_SetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_SetVolume);
        lua_setfield(L, -4, "set_volume");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_GetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_GetPaused);
        lua_setfield(L, -4, "get_paused");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_SetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_SetPaused);
        lua_setfield(L, -4, "set_paused");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_GetMute
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_GetMute);
        lua_setfield(L, -4, "get_mute");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_SetMute
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_SetMute);
        lua_setfield(L, -4, "set_mute");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_StopAllEvents
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_StopAllEvents);
        lua_setfield(L, -4, "stop_all_events");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_GetPortIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_GetPortIndex);
        lua_setfield(L, -4, "get_port_index");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_SetPortIndex
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_SetPortIndex);
        lua_setfield(L, -4, "set_port_index");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_LockChannelGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_LockChannelGroup);
        lua_setfield(L, -4, "lock_channel_group");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_UnlockChannelGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_UnlockChannelGroup);
        lua_setfield(L, -4, "unlock_channel_group");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_GetChannelGroup
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_GetChannelGroup);
        lua_setfield(L, -4, "get_channel_group");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_GetCPUUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_GetCPUUsage);
        lua_setfield(L, -4, "get_cpu_usage");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bus_GetMemoryUsage
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bus_GetMemoryUsage);
        lua_setfield(L, -4, "get_memory_usage");
        #endif
        #ifdef FMODExt_extras_FMOD_STUDIO_BUS
        FMODExt_extras_FMOD_STUDIO_BUS
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_VCA);
        addClassEquality(FMOD_STUDIO_VCA);
        #ifdef FMODExt_func_FMOD_Studio_VCA_IsValid
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_VCA_IsValid);
        lua_setfield(L, -4, "is_valid");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_VCA_GetID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_VCA_GetID);
        lua_setfield(L, -4, "get_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_VCA_GetPath
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_VCA_GetPath);
        lua_setfield(L, -4, "get_path");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_VCA_GetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_VCA_GetVolume);
        lua_setfield(L, -4, "get_volume");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_VCA_SetVolume
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_VCA_SetVolume);
        lua_setfield(L, -4, "set_volume");
        #endif
        #ifdef FMODExt_extras_FMOD_STUDIO_VCA
        FMODExt_extras_FMOD_STUDIO_VCA
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_BANK);
        addClassEquality(FMOD_STUDIO_BANK);
        #ifdef FMODExt_func_FMOD_Studio_Bank_IsValid
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_IsValid);
        lua_setfield(L, -4, "is_valid");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetID);
        lua_setfield(L, -4, "get_id");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetPath
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetPath);
        lua_setfield(L, -4, "get_path");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_Unload
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_Unload);
        lua_setfield(L, -4, "unload");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_LoadSampleData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_LoadSampleData);
        lua_setfield(L, -4, "load_sample_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_UnloadSampleData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_UnloadSampleData);
        lua_setfield(L, -4, "unload_sample_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetLoadingState
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetLoadingState);
        lua_setfield(L, -4, "get_loading_state");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetSampleLoadingState
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetSampleLoadingState);
        lua_setfield(L, -4, "get_sample_loading_state");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetStringCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetStringCount);
        lua_setfield(L, -4, "get_string_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetStringInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetStringInfo);
        lua_setfield(L, -4, "get_string_info");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetEventCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetEventCount);
        lua_setfield(L, -4, "get_event_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetEventList
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetEventList);
        lua_setfield(L, -4, "get_event_list");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetBusCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetBusCount);
        lua_setfield(L, -4, "get_bus_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetBusList
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetBusList);
        lua_setfield(L, -4, "get_bus_list");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetVCACount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetVCACount);
        lua_setfield(L, -4, "get_vca_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetVCAList
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetVCAList);
        lua_setfield(L, -4, "get_vca_list");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_Bank_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_Bank_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_extras_FMOD_STUDIO_BANK
        FMODExt_extras_FMOD_STUDIO_BANK
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_COMMANDREPLAY);
        addClassEquality(FMOD_STUDIO_COMMANDREPLAY);
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_IsValid
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_IsValid);
        lua_setfield(L, -4, "is_valid");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetSystem
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetSystem);
        lua_setfield(L, -4, "get_system");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetLength
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetLength);
        lua_setfield(L, -4, "get_length");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetCommandCount
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetCommandCount);
        lua_setfield(L, -4, "get_command_count");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetCommandInfo
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetCommandInfo);
        lua_setfield(L, -4, "get_command_info");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetCommandString
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetCommandString);
        lua_setfield(L, -4, "get_command_string");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetCommandAtTime
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetCommandAtTime);
        lua_setfield(L, -4, "get_command_at_time");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_SetBankPath
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_SetBankPath);
        lua_setfield(L, -4, "set_bank_path");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_Start
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_Start);
        lua_setfield(L, -4, "start");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_Stop
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_Stop);
        lua_setfield(L, -4, "stop");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_SeekToTime
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_SeekToTime);
        lua_setfield(L, -4, "seek_to_time");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_SeekToCommand
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_SeekToCommand);
        lua_setfield(L, -4, "seek_to_command");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetPaused);
        lua_setfield(L, -4, "get_paused");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_SetPaused
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_SetPaused);
        lua_setfield(L, -4, "set_paused");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetPlaybackState
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetPlaybackState);
        lua_setfield(L, -4, "get_playback_state");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetCurrentCommand
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetCurrentCommand);
        lua_setfield(L, -4, "get_current_command");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_Release
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_Release);
        lua_setfield(L, -4, "release");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_SetFrameCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_SetFrameCallback);
        lua_setfield(L, -4, "set_frame_callback");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_SetLoadBankCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_SetLoadBankCallback);
        lua_setfield(L, -4, "set_load_bank_callback");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_SetCreateInstanceCallback
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_SetCreateInstanceCallback);
        lua_setfield(L, -4, "set_create_instance_callback");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_GetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_GetUserData);
        lua_setfield(L, -4, "get_user_data");
        #endif
        #ifdef FMODExt_func_FMOD_Studio_CommandReplay_SetUserData
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_CommandReplay_SetUserData);
        lua_setfield(L, -4, "set_user_data");
        #endif
        #ifdef FMODExt_extras_FMOD_STUDIO_COMMANDREPLAY
        FMODExt_extras_FMOD_STUDIO_COMMANDREPLAY
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_BANK_INFO);
        addStructEquality(FMOD_STUDIO_BANK_INFO);
        addStructConstructor(FMOD_STUDIO_BANK_INFO, "BANK_INFO", -1);
        /* int size */
        addPropertyGetter(FMOD_STUDIO_BANK_INFO, size, int);
        addPropertySetter(FMOD_STUDIO_BANK_INFO, size, int);
        /* void* userdata */
        /* int userdatalength */
        addPropertyGetter(FMOD_STUDIO_BANK_INFO, userdatalength, int);
        addPropertySetter(FMOD_STUDIO_BANK_INFO, userdatalength, int);
        /* FMOD_FILE_OPEN_CALLBACK opencallback */
        /* FMOD_FILE_CLOSE_CALLBACK closecallback */
        /* FMOD_FILE_READ_CALLBACK readcallback */
        /* FMOD_FILE_SEEK_CALLBACK seekcallback */
        #ifdef FMODExt_extras_FMOD_STUDIO_BANK_INFO
        FMODExt_extras_FMOD_STUDIO_BANK_INFO
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_PARAMETER_ID);
        addStructEquality(FMOD_STUDIO_PARAMETER_ID);
        addStructConstructor(FMOD_STUDIO_PARAMETER_ID, "PARAMETER_ID", -1);
        /* unsigned int data1 */
        addPropertyGetter(FMOD_STUDIO_PARAMETER_ID, data1, unsigned_int);
        addPropertySetter(FMOD_STUDIO_PARAMETER_ID, data1, unsigned_int);
        /* unsigned int data2 */
        addPropertyGetter(FMOD_STUDIO_PARAMETER_ID, data2, unsigned_int);
        addPropertySetter(FMOD_STUDIO_PARAMETER_ID, data2, unsigned_int);
        #ifdef FMODExt_extras_FMOD_STUDIO_PARAMETER_ID
        FMODExt_extras_FMOD_STUDIO_PARAMETER_ID
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_PARAMETER_DESCRIPTION);
        addStructEquality(FMOD_STUDIO_PARAMETER_DESCRIPTION);
        addStructConstructor(FMOD_STUDIO_PARAMETER_DESCRIPTION, "PARAMETER_DESCRIPTION", -1);
        /* const char* name */
        /* FMOD_STUDIO_PARAMETER_ID id */
        addPropertyGetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, id, FMOD_STUDIO_PARAMETER_ID);
        addPropertySetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, id, FMOD_STUDIO_PARAMETER_ID);
        /* float minimum */
        addPropertyGetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, minimum, float);
        addPropertySetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, minimum, float);
        /* float maximum */
        addPropertyGetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, maximum, float);
        addPropertySetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, maximum, float);
        /* float defaultvalue */
        addPropertyGetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, defaultvalue, float);
        addPropertySetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, defaultvalue, float);
        /* FMOD_STUDIO_PARAMETER_TYPE type */
        addPropertyGetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, type, FMOD_STUDIO_PARAMETER_TYPE);
        addPropertySetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, type, FMOD_STUDIO_PARAMETER_TYPE);
        /* FMOD_STUDIO_PARAMETER_FLAGS flags */
        addPropertyGetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, flags, unsigned_int);
        addPropertySetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, flags, unsigned_int);
        /* FMOD_GUID guid */
        addPropertyGetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, guid, FMOD_GUID);
        addPropertySetter(FMOD_STUDIO_PARAMETER_DESCRIPTION, guid, FMOD_GUID);
        #ifdef FMODExt_extras_FMOD_STUDIO_PARAMETER_DESCRIPTION
        FMODExt_extras_FMOD_STUDIO_PARAMETER_DESCRIPTION
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_USER_PROPERTY);
        addStructEquality(FMOD_STUDIO_USER_PROPERTY);
        addStructConstructor(FMOD_STUDIO_USER_PROPERTY, "USER_PROPERTY", -1);
        /* const char* name */
        /* FMOD_STUDIO_USER_PROPERTY_TYPE type */
        addPropertyGetter(FMOD_STUDIO_USER_PROPERTY, type, FMOD_STUDIO_USER_PROPERTY_TYPE);
        addPropertySetter(FMOD_STUDIO_USER_PROPERTY, type, FMOD_STUDIO_USER_PROPERTY_TYPE);
        #ifdef FMODExt_extras_FMOD_STUDIO_USER_PROPERTY
        FMODExt_extras_FMOD_STUDIO_USER_PROPERTY
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES);
        addStructEquality(FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES);
        addStructConstructor(FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES, "PROGRAMMER_SOUND_PROPERTIES", -1);
        /* const char* name */
        /* FMOD_SOUND* sound */
        addPropertyGetter(FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES, sound, ptr_FMOD_SOUND);
        /* int subsoundIndex */
        addPropertyGetter(FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES, subsoundIndex, int);
        addPropertySetter(FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES, subsoundIndex, int);
        #ifdef FMODExt_extras_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES
        FMODExt_extras_FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES);
        addStructEquality(FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES);
        addStructConstructor(FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES, "PLUGIN_INSTANCE_PROPERTIES", -1);
        /* const char* name */
        /* FMOD_DSP* dsp */
        addPropertyGetter(FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES, dsp, ptr_FMOD_DSP);
        #ifdef FMODExt_extras_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES
        FMODExt_extras_FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES);
        addStructEquality(FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES);
        addStructConstructor(FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES, "TIMELINE_MARKER_PROPERTIES", -1);
        /* const char* name */
        /* int position */
        addPropertyGetter(FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES, position, int);
        addPropertySetter(FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES, position, int);
        #ifdef FMODExt_extras_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES
        FMODExt_extras_FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES);
        addStructEquality(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES);
        addStructConstructor(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, "TIMELINE_BEAT_PROPERTIES", -1);
        /* int bar */
        addPropertyGetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, bar, int);
        addPropertySetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, bar, int);
        /* int beat */
        addPropertyGetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, beat, int);
        addPropertySetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, beat, int);
        /* int position */
        addPropertyGetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, position, int);
        addPropertySetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, position, int);
        /* float tempo */
        addPropertyGetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, tempo, float);
        addPropertySetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, tempo, float);
        /* int timesignatureupper */
        addPropertyGetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, timesignatureupper, int);
        addPropertySetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, timesignatureupper, int);
        /* int timesignaturelower */
        addPropertyGetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, timesignaturelower, int);
        addPropertySetter(FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES, timesignaturelower, int);
        #ifdef FMODExt_extras_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES
        FMODExt_extras_FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES);
        addStructEquality(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES);
        addStructConstructor(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES, "TIMELINE_NESTED_BEAT_PROPERTIES", -1);
        /* FMOD_GUID eventid */
        addPropertyGetter(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES, eventid, FMOD_GUID);
        addPropertySetter(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES, eventid, FMOD_GUID);
        /* FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES properties */
        addPropertyGetter(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES, properties, FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES);
        addPropertySetter(FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES, properties, FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES);
        #ifdef FMODExt_extras_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES
        FMODExt_extras_FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_ADVANCEDSETTINGS);
        addStructEquality(FMOD_STUDIO_ADVANCEDSETTINGS);
        addStructConstructor(FMOD_STUDIO_ADVANCEDSETTINGS, "ADVANCEDSETTINGS", -1);
        /* int cbsize */
        addPropertyGetter(FMOD_STUDIO_ADVANCEDSETTINGS, cbsize, int);
        addPropertySetter(FMOD_STUDIO_ADVANCEDSETTINGS, cbsize, int);
        /* unsigned int commandqueuesize */
        addPropertyGetter(FMOD_STUDIO_ADVANCEDSETTINGS, commandqueuesize, unsigned_int);
        addPropertySetter(FMOD_STUDIO_ADVANCEDSETTINGS, commandqueuesize, unsigned_int);
        /* unsigned int handleinitialsize */
        addPropertyGetter(FMOD_STUDIO_ADVANCEDSETTINGS, handleinitialsize, unsigned_int);
        addPropertySetter(FMOD_STUDIO_ADVANCEDSETTINGS, handleinitialsize, unsigned_int);
        /* int studioupdateperiod */
        addPropertyGetter(FMOD_STUDIO_ADVANCEDSETTINGS, studioupdateperiod, int);
        addPropertySetter(FMOD_STUDIO_ADVANCEDSETTINGS, studioupdateperiod, int);
        /* int idlesampledatapoolsize */
        addPropertyGetter(FMOD_STUDIO_ADVANCEDSETTINGS, idlesampledatapoolsize, int);
        addPropertySetter(FMOD_STUDIO_ADVANCEDSETTINGS, idlesampledatapoolsize, int);
        /* unsigned int streamingscheduledelay */
        addPropertyGetter(FMOD_STUDIO_ADVANCEDSETTINGS, streamingscheduledelay, unsigned_int);
        addPropertySetter(FMOD_STUDIO_ADVANCEDSETTINGS, streamingscheduledelay, unsigned_int);
        /* const char* encryptionkey */
        #ifdef FMODExt_extras_FMOD_STUDIO_ADVANCEDSETTINGS
        FMODExt_extras_FMOD_STUDIO_ADVANCEDSETTINGS
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_CPU_USAGE);
        addStructEquality(FMOD_STUDIO_CPU_USAGE);
        addStructConstructor(FMOD_STUDIO_CPU_USAGE, "CPU_USAGE", -1);
        /* float update */
        addPropertyGetter(FMOD_STUDIO_CPU_USAGE, update, float);
        addPropertySetter(FMOD_STUDIO_CPU_USAGE, update, float);
        #ifdef FMODExt_extras_FMOD_STUDIO_CPU_USAGE
        FMODExt_extras_FMOD_STUDIO_CPU_USAGE
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_BUFFER_INFO);
        addStructEquality(FMOD_STUDIO_BUFFER_INFO);
        addStructConstructor(FMOD_STUDIO_BUFFER_INFO, "BUFFER_INFO", -1);
        /* int currentusage */
        addPropertyGetter(FMOD_STUDIO_BUFFER_INFO, currentusage, int);
        addPropertySetter(FMOD_STUDIO_BUFFER_INFO, currentusage, int);
        /* int peakusage */
        addPropertyGetter(FMOD_STUDIO_BUFFER_INFO, peakusage, int);
        addPropertySetter(FMOD_STUDIO_BUFFER_INFO, peakusage, int);
        /* int capacity */
        addPropertyGetter(FMOD_STUDIO_BUFFER_INFO, capacity, int);
        addPropertySetter(FMOD_STUDIO_BUFFER_INFO, capacity, int);
        /* int stallcount */
        addPropertyGetter(FMOD_STUDIO_BUFFER_INFO, stallcount, int);
        addPropertySetter(FMOD_STUDIO_BUFFER_INFO, stallcount, int);
        /* float stalltime */
        addPropertyGetter(FMOD_STUDIO_BUFFER_INFO, stalltime, float);
        addPropertySetter(FMOD_STUDIO_BUFFER_INFO, stalltime, float);
        #ifdef FMODExt_extras_FMOD_STUDIO_BUFFER_INFO
        FMODExt_extras_FMOD_STUDIO_BUFFER_INFO
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_BUFFER_USAGE);
        addStructEquality(FMOD_STUDIO_BUFFER_USAGE);
        addStructConstructor(FMOD_STUDIO_BUFFER_USAGE, "BUFFER_USAGE", -1);
        /* FMOD_STUDIO_BUFFER_INFO studiocommandqueue */
        addPropertyGetter(FMOD_STUDIO_BUFFER_USAGE, studiocommandqueue, FMOD_STUDIO_BUFFER_INFO);
        addPropertySetter(FMOD_STUDIO_BUFFER_USAGE, studiocommandqueue, FMOD_STUDIO_BUFFER_INFO);
        /* FMOD_STUDIO_BUFFER_INFO studiohandle */
        addPropertyGetter(FMOD_STUDIO_BUFFER_USAGE, studiohandle, FMOD_STUDIO_BUFFER_INFO);
        addPropertySetter(FMOD_STUDIO_BUFFER_USAGE, studiohandle, FMOD_STUDIO_BUFFER_INFO);
        #ifdef FMODExt_extras_FMOD_STUDIO_BUFFER_USAGE
        FMODExt_extras_FMOD_STUDIO_BUFFER_USAGE
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_SOUND_INFO);
        addStructEquality(FMOD_STUDIO_SOUND_INFO);
        addStructConstructor(FMOD_STUDIO_SOUND_INFO, "SOUND_INFO", -1);
        /* const char* name_or_data */
        /* FMOD_MODE mode */
        addPropertyGetter(FMOD_STUDIO_SOUND_INFO, mode, unsigned_int);
        addPropertySetter(FMOD_STUDIO_SOUND_INFO, mode, unsigned_int);
        /* FMOD_CREATESOUNDEXINFO exinfo */
        addPropertyGetter(FMOD_STUDIO_SOUND_INFO, exinfo, FMOD_CREATESOUNDEXINFO);
        addPropertySetter(FMOD_STUDIO_SOUND_INFO, exinfo, FMOD_CREATESOUNDEXINFO);
        /* int subsoundindex */
        addPropertyGetter(FMOD_STUDIO_SOUND_INFO, subsoundindex, int);
        addPropertySetter(FMOD_STUDIO_SOUND_INFO, subsoundindex, int);
        #ifdef FMODExt_extras_FMOD_STUDIO_SOUND_INFO
        FMODExt_extras_FMOD_STUDIO_SOUND_INFO
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_COMMAND_INFO);
        addStructEquality(FMOD_STUDIO_COMMAND_INFO);
        addStructConstructor(FMOD_STUDIO_COMMAND_INFO, "COMMAND_INFO", -1);
        /* const char* commandname */
        /* int parentcommandindex */
        addPropertyGetter(FMOD_STUDIO_COMMAND_INFO, parentcommandindex, int);
        addPropertySetter(FMOD_STUDIO_COMMAND_INFO, parentcommandindex, int);
        /* int framenumber */
        addPropertyGetter(FMOD_STUDIO_COMMAND_INFO, framenumber, int);
        addPropertySetter(FMOD_STUDIO_COMMAND_INFO, framenumber, int);
        /* float frametime */
        addPropertyGetter(FMOD_STUDIO_COMMAND_INFO, frametime, float);
        addPropertySetter(FMOD_STUDIO_COMMAND_INFO, frametime, float);
        /* FMOD_STUDIO_INSTANCETYPE instancetype */
        addPropertyGetter(FMOD_STUDIO_COMMAND_INFO, instancetype, FMOD_STUDIO_INSTANCETYPE);
        addPropertySetter(FMOD_STUDIO_COMMAND_INFO, instancetype, FMOD_STUDIO_INSTANCETYPE);
        /* FMOD_STUDIO_INSTANCETYPE outputtype */
        addPropertyGetter(FMOD_STUDIO_COMMAND_INFO, outputtype, FMOD_STUDIO_INSTANCETYPE);
        addPropertySetter(FMOD_STUDIO_COMMAND_INFO, outputtype, FMOD_STUDIO_INSTANCETYPE);
        /* unsigned int instancehandle */
        addPropertyGetter(FMOD_STUDIO_COMMAND_INFO, instancehandle, unsigned_int);
        addPropertySetter(FMOD_STUDIO_COMMAND_INFO, instancehandle, unsigned_int);
        /* unsigned int outputhandle */
        addPropertyGetter(FMOD_STUDIO_COMMAND_INFO, outputhandle, unsigned_int);
        addPropertySetter(FMOD_STUDIO_COMMAND_INFO, outputhandle, unsigned_int);
        #ifdef FMODExt_extras_FMOD_STUDIO_COMMAND_INFO
        FMODExt_extras_FMOD_STUDIO_COMMAND_INFO
        #endif
    endStruct();
    beginStruct(FMOD_STUDIO_MEMORY_USAGE);
        addStructEquality(FMOD_STUDIO_MEMORY_USAGE);
        addStructConstructor(FMOD_STUDIO_MEMORY_USAGE, "MEMORY_USAGE", -1);
        /* int exclusive */
        addPropertyGetter(FMOD_STUDIO_MEMORY_USAGE, exclusive, int);
        addPropertySetter(FMOD_STUDIO_MEMORY_USAGE, exclusive, int);
        /* int inclusive */
        addPropertyGetter(FMOD_STUDIO_MEMORY_USAGE, inclusive, int);
        addPropertySetter(FMOD_STUDIO_MEMORY_USAGE, inclusive, int);
        /* int sampledata */
        addPropertyGetter(FMOD_STUDIO_MEMORY_USAGE, sampledata, int);
        addPropertySetter(FMOD_STUDIO_MEMORY_USAGE, sampledata, int);
        #ifdef FMODExt_extras_FMOD_STUDIO_MEMORY_USAGE
        FMODExt_extras_FMOD_STUDIO_MEMORY_USAGE
        #endif
    endStruct();
    

    
        #ifdef FMODExt_func_FMOD_Memory_Initialize
        lua_pushcfunction(L, &FMODExt_func_FMOD_Memory_Initialize);
        lua_setfield(L, -2 - 1, "memory_initialize");
        #endif
    
        #ifdef FMODExt_func_FMOD_Memory_GetStats
        lua_pushcfunction(L, &FMODExt_func_FMOD_Memory_GetStats);
        lua_setfield(L, -2 - 1, "memory_get_stats");
        #endif
    
        #ifdef FMODExt_func_FMOD_Debug_Initialize
        lua_pushcfunction(L, &FMODExt_func_FMOD_Debug_Initialize);
        lua_setfield(L, -2 - 1, "debug_initialize");
        #endif
    
        #ifdef FMODExt_func_FMOD_File_SetDiskBusy
        lua_pushcfunction(L, &FMODExt_func_FMOD_File_SetDiskBusy);
        lua_setfield(L, -2 - 1, "file_set_disk_busy");
        #endif
    
        #ifdef FMODExt_func_FMOD_File_GetDiskBusy
        lua_pushcfunction(L, &FMODExt_func_FMOD_File_GetDiskBusy);
        lua_setfield(L, -2 - 1, "file_get_disk_busy");
        #endif
    
        #ifdef FMODExt_func_FMOD_Thread_SetAttributes
        lua_pushcfunction(L, &FMODExt_func_FMOD_Thread_SetAttributes);
        lua_setfield(L, -2 - 1, "thread_set_attributes");
        #endif
    
        #ifdef FMODExt_func_FMOD_System_Create
        lua_pushcfunction(L, &FMODExt_func_FMOD_System_Create);
        lua_setfield(L, -2 - 1, "system_create");
        #endif
    
        #ifdef FMODExt_func_FMOD_Studio_ParseID
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_ParseID);
        lua_setfield(L, -1 - 1, "parse_id");
        #endif
    
        #ifdef FMODExt_func_FMOD_Studio_System_Create
        lua_pushcfunction(L, &FMODExt_func_FMOD_Studio_System_Create);
        lua_setfield(L, -1 - 1, "system_create");
        #endif
    

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
