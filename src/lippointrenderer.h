#ifndef LIPPOINTRENDERER_H
#define LIPPOINTRENDERER_H

#include "lipfeaturerenderer.h"
#include "vector/lippoint.h"

class LIPPointRenderer : public LIPFeatureRenderer
{
public:
    LIPPointRenderer(LIPPoint* point);

    // LIPFeatureRenderer interface
public:
    void renderFeature();

private:
    LIPPoint *mPoint;
};

#endif // LIPPOINTRENDERER_H
