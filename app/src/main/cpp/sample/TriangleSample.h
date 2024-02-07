//
// Created by 86422 on 2024/2/1.
//

#include "SampleBaseGL.h"

class TriangleSample : public SampleBaseGL
{
public:
    TriangleSample();
    virtual ~TriangleSample();

    virtual void Init();
    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

};
