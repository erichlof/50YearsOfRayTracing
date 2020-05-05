#include "RayCastingRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

void RayCastingRenderer::_renderThread(MyScene::Ptr scene,
                                       MyCamera::Ptr camera) {
  mEyePos = camera->getEyePos();
  camera->getFocalPlane(mFocalPlaneH, mFocalPlaneV, mFocalPlaneLeftTop);

  MyScene* pScene = scene.get();
  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (!mRuning) break;

      _drawSinglePixel(x, y, pScene);
      mPixelCount++;
    }  // end of for(x)
}

Ray RayCastingRenderer::_generateEyeRay(int x, int y) {
  float s = (x + 0.5f) / (float)(mFrameWidth);
  float t = (y + 0.5f) / (float)(mFrameHeight);

  glm::vec3 origin = mEyePos;
  return Ray(origin,
             mFocalPlaneLeftTop + s * mFocalPlaneH - t * mFocalPlaneV - origin);
}

Ray RayCastingRenderer::_generateShadowRay(const glm::vec3& point) {
  glm::vec3 L = glm::normalize(mLightPos - point);
  return Ray(point, L);
}

void RayCastingRenderer::_drawSinglePixel(int x, int y, MyScene* pScene) {
  constexpr float fMax = std::numeric_limits<float>::max();

  HitRecord hitRec;
  Ray eyeRay = _generateEyeRay(x, y);
  bool bHit = pScene->hit(eyeRay, 0, fMax, hitRec);

  if (!bHit) return;

  glm::vec4 color;
  Ray shadowRay = _generateShadowRay(hitRec.p);
  HitRecord hitRec2;
  constexpr float SHADOW_E = 0.1f;
  bool bShadow = pScene->hit(shadowRay, SHADOW_E, fMax, hitRec2);
  if (bShadow) {
    color = glm::vec4(0, 0, 0, 1);
  } else {
    glm::vec3 L = glm::normalize(mLightPos - hitRec.p);
    float c = std::max(0.0f, glm::dot(hitRec.normal, L));
    color = glm::vec4(c, c, c, 1);
  }

  _writePixel(x, y, color);
}