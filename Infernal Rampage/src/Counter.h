#pragma once

#include "GameObject.h"

class Counter : public GameObject
{
public:
    Counter(Transform _ObjectTransform);

private:
    int Number;

public:
    int GetNumber() { return Number; }
    void SetNumber(int n) { Number = n; }
    virtual glm::vec2 GetUVOffset() override;
    virtual bool GetIsUnlit() override { return true; }
};
