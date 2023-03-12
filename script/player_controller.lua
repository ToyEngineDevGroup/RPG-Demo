local PlayerController = {
    name = "PlayerController",
    elapsed_time = 0,
    curr_anime = "RunDown",
    last_anime = "RunDown",
    speed = 1.0,
    is_attacking = false,
}

local AnimeTable = {
    RunUp       = { bias = 6, frame_num = 6 },
    RunDown     = { bias = 18, frame_num = 6 },
    RunLeft     = { bias = 12, frame_num = 6 },
    RunRight    = { bias = 0, frame_num = 6 },
    AttackUp    = { bias = 28, frame_num = 4 },
    AttackDown  = { bias = 36, frame_num = 4 },
    AttackLeft  = { bias = 32, frame_num = 4 },
    AttackRight = { bias = 24, frame_num = 4 },
}

function PlayerController:onCreate()
end

function PlayerController:onDestroy()
end

function PlayerController:movePlayerShadow()
    self.parent:GetParentScene():GetEntityByName("player_shadow"):GetTransform().translation =
        self.parent:GetTransform().translation - Vector3:new(0.01, 0.17, 0)
end

function PlayerController:generateHitBox()
    self.parent:GetParentScene():CreateEntity("player_hitbox")
    self.parent:GetParentScene():GetEntityByName("player_hitbox"):AddRigidbody2D()
    self.parent:GetParentScene():GetEntityByName("player_hitbox"):GetRigidbody2D().type = BodyType.Static
    self.parent:GetParentScene():GetEntityByName("player_hitbox"):GetRigidbody2D().is_fixed_rotation = true

    local offset = nil

    if self.curr_anime == "AttackUp" then
        offset = Vector3:new(0.0, -0.4, 0.0)
        self.parent:GetParentScene():GetEntityByName("player_hitbox"):GetRigidbody2D():CreateBox(0.4, 0.3)
    elseif self.curr_anime == "AttackDown" then
        offset = Vector3:new(0.0, 0.4, 0.0)
        self.parent:GetParentScene():GetEntityByName("player_hitbox"):GetRigidbody2D():CreateBox(0.4, 0.3)
    elseif self.curr_anime == "AttackLeft" then
        offset = Vector3:new(0.4, 0.0, 0.0)
        self.parent:GetParentScene():GetEntityByName("player_hitbox"):GetRigidbody2D():CreateBox(0.3, 0.4)
    elseif self.curr_anime == "AttackRight" then
        offset = Vector3:new(-0.4, 0.0, 0.0)
        self.parent:GetParentScene():GetEntityByName("player_hitbox"):GetRigidbody2D():CreateBox(0.3, 0.4)
        self.curr_anime = "AttackRight"
    end

    self.parent:GetParentScene():GetEntityByName("player_hitbox"):GetTransform().translation =
        self.parent:GetTransform().translation - offset
end

function PlayerController:destroyHitBox()
    -- self.parent:GetParentScene():DestroyEntity(self.parent:GetParentScene():GetEntityByName("player_hitbox"))
end

function PlayerController:onUpdate(timestep)
    self:movePlayerShadow()
    self.parent:GetRigidbody2D().linear_velocity = Vector2:new(0, 0)

    if not self.is_attacking then
        if Input.isKeyPressed(string.byte('W')) then
            self.last_anime = self.curr_anime
            self.curr_anime = "RunUp"
            self.parent:GetRigidbody2D().linear_velocity.y = self.speed
        end
        if Input.isKeyPressed(string.byte('S')) then
            self.last_anime = self.curr_anime
            self.curr_anime = "RunDown"
            self.parent:GetRigidbody2D().linear_velocity.y = -self.speed
        end
        if Input.isKeyPressed(string.byte('D')) then
            self.last_anime = self.curr_anime
            self.curr_anime = "RunRight"
            self.parent:GetRigidbody2D().linear_velocity.x = self.speed
        end
        if Input.isKeyPressed(string.byte('A')) then
            self.last_anime = self.curr_anime
            self.curr_anime = "RunLeft"
            self.parent:GetRigidbody2D().linear_velocity.x = -self.speed
        end
    end

    if Input.isKeyPressed(string.byte('J')) then
        if not self.is_attacking then
            self.is_attacking = true
            self.last_anime = self.curr_anime
            if self.curr_anime == "RunUp" then
                self.curr_anime = "AttackUp"
            elseif self.curr_anime == "RunDown" then
                self.curr_anime = "AttackDown"
            elseif self.curr_anime == "RunLeft" then
                self.curr_anime = "AttackLeft"
            elseif self.curr_anime == "RunRight" then
                self.curr_anime = "AttackRight"
            end
            -- self:generateHitBox()
        end
    end

    self.elapsed_time = self.elapsed_time + timestep
    if self.elapsed_time > 0.1 then
        if AnimeTable[self.curr_anime].frame_num ~= 4 and self.parent:GetRigidbody2D().linear_velocity.x == 0 and self.parent:GetRigidbody2D().linear_velocity.y == 0 then
            -- Running
            self.parent:GetTile().coord_x = AnimeTable[self.curr_anime].bias
        else
            -- Attacking
            self.parent:GetTile().coord_x =
                ((self.parent:GetTile().coord_x + 1) % AnimeTable[self.curr_anime].frame_num) +
                AnimeTable[self.curr_anime].bias
            if AnimeTable[self.curr_anime].frame_num == 4 and (self.parent:GetTile().coord_x + 1) % AnimeTable[self.curr_anime].frame_num == 0 then
                self.is_attacking = false
                if self.curr_anime == "AttackUp" then
                    self.last_anime = "AttackUp"
                    self.curr_anime = "RunUp"
                elseif self.curr_anime == "AttackDown" then
                    self.last_anime = "AttackDown"
                    self.curr_anime = "RunDown"
                elseif self.curr_anime == "AttackLeft" then
                    self.last_anime = "AttackLeft"
                    self.curr_anime = "RunLeft"
                elseif self.curr_anime == "AttackRight" then
                    self.last_anime = "AttackRight"
                    self.curr_anime = "RunRight"
                end
                self:destroyHitBox()
            end
        end
        self.elapsed_time = self.elapsed_time - 0.1
    end
end

function PlayerController:onCollisionEnter(entity)
    Debug.LogInfo(entity:GetName() .. "Enter")
end

function PlayerController:onCollisionExit(entity)
    Debug.LogInfo(entity:GetName() .. "Exit")
end

return PlayerController
