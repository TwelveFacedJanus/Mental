local sprite

function main()
    sprite = create_sprite2d()
    sprite:set_position(0.0, 0.0, 0.0)
    sprite:apply_texture("../Textures/character2.png")
    sprite:add_script("../Scripts/sprite_component.lua")

end

function process(dt)
    sprite:flip_h()
end
