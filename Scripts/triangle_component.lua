angle = (angle or 0)
pos_x = (pos_x or 0)
function main()
    rotate(2, 2)
    apply_texture("../Textures/character2.png")
    set_position(-20, 0, 0)
end

function process(delta)
    angle = angle + delta
    pos_x = pos_x + delta
    reset_model_matrix()
    set_position(-1, 0, 0)
    rotate(angle*10, 2)
end
