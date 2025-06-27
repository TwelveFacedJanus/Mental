local frame_count = 10 -- число кадров
local frame_time = 0.1
local timer = 0
local current_frame = 1

function main()
    for i = 0, frame_count - 1 do
        self:add_texture(string.format("./Textures/forest/%d_forest.png", i))
    end
    self:set_position(0.0, 0.0, 0.0)
    self:set_scale(4.0, 4.0, 4.0)
    self:set_texture_index(1)
end

function process(dt)
    timer = timer + dt
    if timer >= frame_time then
        timer = timer - frame_time
        current_frame = (current_frame % frame_count) + 1
        self:set_texture_index(current_frame)
    end
end