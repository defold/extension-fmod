---
title: Defold FMOD extension API documentation
brief: This manual covers how to integrate a game with the FMOD Studio.
---

# Defold FMOD extension API documentation

This extension provides an interface to integrate a game with the adaptive audio engine FMOD Studio.

## Load banks
```lua
fmod.studio.system:load_bank_memory(resource.load("/banks/Master Bank.bank"), fmod.STUDIO_LOAD_BANK_NORMAL)
fmod.studio.system:load_bank_memory(resource.load("/banks/Master Bank.strings.bank"), fmod.STUDIO_LOAD_BANK_NORMAL)
fmod.studio.system:load_bank_memory(resource.load("/banks/Vehicles.bank"), fmod.STUDIO_LOAD_BANK_NORMAL)

local event_description = fmod.studio.system:get_event("event:/Vehicles/Basic Engine")
local event = event_description:create_instance()
event:start()
```

(...)
