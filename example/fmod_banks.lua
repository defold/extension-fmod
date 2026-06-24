local M = {}

-- FMOD Studio events live in bank files. Each example needs the Master bank,
-- the strings bank for resolving human-readable paths such as
-- "event:/Vehicles/Basic Engine", and any content banks that contain the
-- event data itself.
--
-- This module keeps bank loading shared between collection examples. Defold
-- collection proxies can unload and reload scripts while the FMOD Studio
-- system is still alive, so blindly calling load_bank_memory every time can
-- produce "bank already loaded" errors when switching examples.
local banks = {}

local BANKS = {
	master = {
		resource = "/example/banks/Master Bank.bank",
		path = "bank:/Master Bank",
	},
	strings = {
		resource = "/example/banks/Master Bank.strings.bank",
		path = "bank:/Master Bank.strings",
	},
	vehicles = {
		resource = "/example/banks/Vehicles.bank",
		path = "bank:/Vehicles",
	},
}

local function load_bank(id)
	-- Return the cached Lua handle first. This is the common path after the
	-- first example has loaded a bank.
	if banks[id] then
		return banks[id]
	end

	local bank = BANKS[id]
	if not bank then
		error("Unknown FMOD bank: " .. tostring(id))
	end

	-- If the bank was loaded by a previous script instance, ask FMOD for the
	-- existing bank instead of loading the same memory resource again.
	local ok_existing, existing = pcall(function()
		return fmod.studio.system:get_bank(bank.path)
	end)
	if ok_existing and existing then
		banks[id] = existing
		return existing
	end

	-- load_bank_memory loads a Defold custom resource into FMOD. The bank files
	-- are listed under custom_resources in game.project so resource.load can
	-- access them at runtime.
	local ok, result = pcall(function()
		return fmod.studio.system:load_bank_memory(resource.load(bank.resource), fmod.STUDIO_LOAD_BANK_NORMAL)
	end)

	if ok then
		banks[id] = result
		return result
	end

	error(result)
end

function M.load(ids)
	-- Keep the public API small for the examples: pass a list of bank ids and
	-- this helper handles caching, existing FMOD banks, and resource loading.
	for i = 1, #ids do
		load_bank(ids[i])
	end
end

return M
