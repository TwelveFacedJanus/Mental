local Character = require('Scripts/Character')

local frame_count = 8 -- число кадров
local frame_time = 0.1
local timer = 0
local current_frame = 1

local character_state = {
    speed = 1,
    health = 100,
    damage = 10,
    position = { x = 0, y = 0, z = 0 },
    flipped_h = false,
    flipped_v = false,
    is_jumping = false,
    jump_velocity = 0,
    gravity = -20,
    ground_y = 0,
    current_flipped = false
}

function main()
    self:apply_texture("./Textures/Characters/Kiselek/tile000.png")
    self:set_scale(3.0, 2.0, 1.0)
end

function process(delta)
    reset_model_matrix()
    self:set_scale(1.0, 2.0, 1.0)
    
    timer = timer + delta
    if timer >= frame_time then
        timer = timer - frame_time
        current_frame = (current_frame % frame_count) + 1
        self:set_texture_index(current_frame)
    end
    input = get_glfw_action()
    Character.platformer_movement(self, character_state, delta, input);
    self:set_position(character_state.position.x, character_state.position.y, character_state.position.z)
end



