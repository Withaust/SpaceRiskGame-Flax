#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Math/Math.h>
#include <Engine/Core/Math/Vector2.h>

API_CLASS() class GAME_API MathHelper : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE(MathHelper);

    API_FUNCTION() static bool PointInPolygon(Float2 point, Array<Float2> polygon)
    {
        int num_vertices = polygon.Count();

        float x = point.X;
        float y = point.Y;

        bool inside = false;

        Float2 p1 = polygon[0];
        Float2 p2;

        for (int i = 1; i <= num_vertices; i++)
        {
            p2 = polygon[i % num_vertices];

            if (y > Math::Min(p1.Y, p2.Y) && y <= Math::Max(p1.Y, p2.Y) && x <= Math::Max(p1.X, p2.X))
            {
                double x_intersection = (y - p1.Y) * (p2.X - p1.X) / (p2.Y - p1.Y) + p1.X;

                if (p1.X == p2.X || x <= x_intersection)
                {
                    inside = !inside;
                }
            }

            p1 = p2;
        }

        return inside;
    }

};
