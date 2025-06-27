local Character = {}

Character.speed = 1
Character.health = 100
Character.damage = 10
Character.position = { x = 0, y = 0, z = 0 }
Character.flipped_h = false
Character.flipped_v = false
Character.is_jumping = false
Character.jump_velocity = 0
Character.gravity = -20
Character.ground_y = 0
Character.current_flipped = false

function Character.platformer_movement(self, state, delta, input)
    local dx, dy = 0, 0
    for i, key in ipairs(input) do
        if key == "Key_W" then dy = dy + 1 end
        if key == "Key_S" then dy = dy - 1 end
        if key == "Key_A" then dx = dx - 1 end
        if key == "Key_D" then dx = dx + 1 end
        if key == "Key_SPACE" and not state.is_jumping then
            state.is_jumping = true
            state.jump_velocity = 3
            state.ground_y = state.position.y
        end
    end
    if dx ~= 0 or dy ~= 0 then
        local len = math.sqrt(dx*dx + dy*dy)
        dx, dy = dx/len, dy/len
    end
    -- Определяем направление взгляда
    if dx < 0 then
        state.flipped_h = false
    elseif dx > 0 then
        state.flipped_h = true
    end
    if state.flipped_h ~= state.current_flipped then
        -- self:flip_h() -- если есть функция флипа, раскомментируйте
        state.current_flipped = state.flipped_h
    end
    -- Прыжок и гравитация
    if state.is_jumping then
        state.position.y = state.position.y + state.jump_velocity * delta
        state.jump_velocity = state.jump_velocity + state.gravity * delta
        if state.jump_velocity < 0 and state.position.y <= state.ground_y then
            state.is_jumping = false
            state.jump_velocity = 0
            state.position.y = state.ground_y
        end
    else
        state.position.y = state.position.y + state.speed * dy * delta
    end
    state.position.x = state.position.x + state.speed * dx * delta
    -- state.set_position(state.position.x, state.position.y, state.position.z) -- если есть функция установки позиции
    -- export_sprite_position(state.position.x, state.position.y, state.position.z) -- если есть экспорт
end

return Character