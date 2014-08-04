#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

struct RenderConfig
{
    RenderConfig()
    {
        EnableMSAAx4        = true;
        ForceSingleThreaded = true;
    }

    bool EnableMSAAx4;
    bool ForceSingleThreaded;

    const RenderConfig& operator=(RenderConfig& rhs)
    {
        EnableMSAAx4        = rhs.EnableMSAAx4;
        ForceSingleThreaded = rhs.ForceSingleThreaded;

        return *this;
    }
};

#endif