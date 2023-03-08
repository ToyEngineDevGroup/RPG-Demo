local PlayerController = {
    name = "PlayerController",
    elapsed_time = 0,
    bias = 18,
    speed = 1.0
}

function PlayerController:onCreate()
    Debug.LogInfo("player create")
end

function PlayerController:onDestroy()
    Debug.LogInfo("player destroy")
end

function PlayerController:onUpdate(timestep)
    if Input.isKeyPressed(string.byte('W')) then
        self.bias = 6
        self.parent:GetTransform().translation.y = self.parent:GetTransform().translation.y + self.speed * timestep
    end
    if Input.isKeyPressed(string.byte('S')) then
        self.bias = 18
        self.parent:GetTransform().translation.y = self.parent:GetTransform().translation.y - self.speed * timestep
    end
    if Input.isKeyPressed(string.byte('D')) then
        self.bias = 0
        self.parent:GetTransform().translation.x = self.parent:GetTransform().translation.x + self.speed * timestep
    end
    if Input.isKeyPressed(string.byte('A')) then
        self.bias = 12
        self.parent:GetTransform().translation.x = self.parent:GetTransform().translation.x - self.speed * timestep
    end

    self.elapsed_time = self.elapsed_time + timestep
    if self.elapsed_time > 0.1 then
        self.parent:GetTile().coord_x = ((self.parent:GetTile().coord_x + 1) % 6) + self.bias

        self.elapsed_time = self.elapsed_time - 0.1
    end
end

return PlayerController
