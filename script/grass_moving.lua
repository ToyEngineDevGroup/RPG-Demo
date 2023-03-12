local GrassMoving = {
    name = "GrassMoving",
    elapsed_time = 0,
    speed = 1.0,
    initial_angle = 0.0,
}

function GrassMoving:onCreate()
    local initial_pos = self.parent:GetTransform().translation
    if initial_pos.x > 0 then
        self.initial_angle = math.acos(initial_pos.x / 2)
    else
        self.initial_angle = -math.acos(initial_pos.x / 2)
    end

    Debug.Log(string.format("Name: %s, Degree: %f", self.parent:GetName(), self.initial_angle))
end

function GrassMoving:onDestroy()
end

function GrassMoving:onUpdate(timestep)
    self.elapsed_time = self.elapsed_time + timestep
    self.parent:GetTransform().translation = Vector3:new(
        2.0 * math.cos(self.initial_angle + self.elapsed_time),
        2.0 * math.sin(self.initial_angle + self.elapsed_time), 0.0)
end

return GrassMoving
