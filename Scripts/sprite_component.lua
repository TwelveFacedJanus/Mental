angle = (angle or 0)
speed = (speed or 1)
position_x = (position_x or 0)
position_y = (position_y or 0)

flipped = (flipped or false)
current_flipped = (current_flipped or false)
jumping = (jumping or false)
jump_velocity = (jump_velocity or 0)
gravity = (gravity or -20)
ground_y = (ground_y or position_y)
local frame_count = 8 -- число кадров
local frame_time = 0.1
local timer = 0
local current_frame = 1

function main()
    self:apply_texture("../Textures/Characters/Kiselek/tile000.png")
    self:set_scale(3.0, 2.0, 1.0)
end

function process(delta)
    reset_model_matrix() -- сбрасываем все трансформации
    -- self:set_rotation(angle) -- Удалено, чтобы не было ошибки и наклона
    self:set_scale(1.0, 2.0, 1.0)
    
    timer = timer + delta
    if timer >= frame_time then
        timer = timer - frame_time
        current_frame = (current_frame % frame_count) + 1 -- current_frame всегда от 1 до frame_count
        self:set_texture_index(current_frame)
    end

    local input = get_glfw_action()
    local dx, dy = 0, 0
    for i, key in ipairs(input) do
        if key == "Key_W" then dy = dy + 1 end
        if key == "Key_S" then dy = dy - 1 end
        if key == "Key_A" then dx = dx - 1 end
        if key == "Key_D" then dx = dx + 1 end
        if key == "Key_SPACE" and not jumping then
            jumping = true
            jump_velocity = 3 -- скорость прыжка вверх
            ground_y = position_y -- запоминаем высоту старта прыжка
        end
    end
    if dx ~= 0 or dy ~= 0 then
        local len = math.sqrt(dx*dx + dy*dy)
        dx, dy = dx/len, dy/len
    end
    -- Определяем направление взгляда
    if dx < 0 then
        flipped = false
    elseif dx > 0 then
        flipped = true
    end
    if flipped ~= current_flipped then
        self:flip_h()
        current_flipped = flipped
    end
    -- Прыжок и гравитация
    if jumping then
        position_y = position_y + jump_velocity * delta
        jump_velocity = jump_velocity + gravity * delta
        if jump_velocity < 0 and position_y <= ground_y then
            jumping = false
            jump_velocity = 0
            position_y = ground_y -- возвращаем на исходную высоту
        end
    else
        position_y = position_y + speed * dy * delta
    end
    position_x = position_x + speed * dx * delta
    set_position(position_x, position_y, 0)
    export_sprite_position(position_x, position_y, 0)
end


