#include "GameObject.h"

class Sky : public GameObject
{
public:
    Sky();

public:
    virtual bool GetIsUnlit() override { return true; }
};
