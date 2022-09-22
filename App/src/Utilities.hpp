#pragma once

namespace Utils
{
  float Interpol(float srcMin, float srcMax, float tarMin, float tarMax, float ref)
  {
    if (ref <= srcMin)
      return tarMin;

    if (ref >= srcMax)
      return tarMax;

    //  srcStep       tarStep
    // ---------- == --------------
    //  srcTotal      tarTotal
    //
    // tarStep = x - tarMin
    // x = tarStep + tarMin

    float srcTotal = srcMax - srcMin;
    float srcStep = ref - srcMin;

    float tarTotal = tarMax - tarMin;
    float tarStep = (srcStep * tarTotal) / srcTotal;

    return tarStep + tarMin;
  }
}