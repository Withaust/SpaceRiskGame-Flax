#include "PlayerSpawnGroup.h"

PlayerSpawnGroup::PlayerSpawnGroup(const SpawnParams& params)
    : Script(params)
{
}

void PlayerSpawnGroup::OnEnable()
{
    for (int i = 0; i < Spawns.Count(); ++i)
    {
        PlayerRespawn::Get()->Register(Spawns[i]);
    }
}

void PlayerSpawnGroup::OnDisable()
{
    for (int i = 0; i < Spawns.Count(); ++i)
    {
        PlayerRespawn::Get()->Unregister(Spawns[i]);
    }
}

#if USE_EDITOR
void PlayerSpawnGroup::OnDebugDrawSelected()
{
    const Transform& trans = GetActor()->GetTransform();
    for (int i = 0; i < Spawns.Count(); ++i)
    {
        if (!Spawns[i])
        {
            continue;
        }
        DebugDraw::DrawLine(trans.Translation, Spawns[i]->GetActor()->GetTransform().Translation, Color::White, 0.0f, false);
    }
}
#endif
