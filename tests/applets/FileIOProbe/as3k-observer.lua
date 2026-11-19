-- Observer for scripts/as3k-applet-test (SamDelorean/mame-as3k, as3k-mame0289-dev).
-- Answers the fresh-NVRAM prompt, selects the only applet, waits for FileIOProbe's
-- result line and prints it. Every LCD change is printed so a failed boot is
-- diagnosable from the report JSON (emulator_log).
local machine = manager.machine
local frame, last, done = 0, "", false

local function lcd()
    local rows = {}
    for _, tag in ipairs({":ks0066_0", ":ks0066_1"}) do
        local index = assert(machine.devices[tag].items["0/m_ddram"], "missing LCD DDRAM")
        local ddram = emu.item(index)
        rows[#rows + 1] = ddram:read_block(0, 40)
        rows[#rows + 1] = ddram:read_block(0x40, 40)
    end
    return (table.concat(rows, "\n"):gsub("%z", " "))
end

local function finish(tag)
    machine.video:snapshot()
    print(tag)
    done = true
    machine:exit()
end

fileio_observer = emu.register_frame_done(function()
    frame = frame + 1
    if done or frame % 20 ~= 0 then return end
    local ok, screen = pcall(lcd)
    if not ok then return finish("OBSERVER ERROR " .. tostring(screen)) end
    if screen ~= last then
        print(string.format("LCD@%d\n%s\n--", frame, screen))
        last = screen
    end
    local result = screen:match("FILEIO[^\n]*")
    if result then
        local ids = screen:match("ids[^\n]*") or ""
        return finish("RESULT " .. result .. " | " .. ids)
    end
    if frame <= 600 and frame % 100 == 60 then machine.natkeyboard:post("y") end
    if frame >= 200 and frame % 300 == 200 then machine.natkeyboard:post("\r") end
end)
