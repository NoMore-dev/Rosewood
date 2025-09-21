#pragma once

namespace Rosewood
{
    enum class ShaderStage
    {
        Vertex, Fragment, Geometry, Hull, Domain
    };

    static const std::string& ShaderStageString(ShaderStage stage)
    {
        static const std::string vertexStageString = "VERTEX";
        static const std::string fragmentStageString = "FRAGMENT";
        static const std::string geometryStageString = "GEOMETRY";
        static const std::string hullStageString = "HULL";
        static const std::string domainStageString = "DOMAIN";

        switch (stage)
        {
        case Rosewood::ShaderStage::Vertex:
            return vertexStageString;
        case Rosewood::ShaderStage::Fragment:
            return fragmentStageString;
        case Rosewood::ShaderStage::Geometry:
            return geometryStageString;
        case Rosewood::ShaderStage::Hull:
            return hullStageString;
        case Rosewood::ShaderStage::Domain:       
            return domainStageString;
        default:
            break;
        }

        RW_CORE_ASSERT(false, "Invalid shader stage!");

        static const std::string stageString = "NONE";
        return stageString;
    }

}