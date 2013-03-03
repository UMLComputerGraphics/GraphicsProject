/*
 * SceneGraph.cpp
 *
 *  Created on: Dec 11, 2012
 *      Author: soujiroboi
 */

#include "SceneGraph.h"

SceneGraph::SceneGraph(Model *_model, mat4 _prepareRotationMatrix, mat4 _modelOffset) {
  model = _model;
  prepareRotationMatrix = _prepareRotationMatrix;
  applyRotationMatrix = Translate(0.0, 0.0, 0.0);
  modelOffset = _modelOffset;
}

SceneGraph::~SceneGraph() {}

Model* SceneGraph::getModel() {
  return model;
}

mat4 SceneGraph::getPrepareRotationMatrix() {
  return prepareRotationMatrix;
}

mat4 SceneGraph::getApplyRotationMatrix() {
  return applyRotationMatrix;
}

mat4 SceneGraph::getModelOffset() {
  return modelOffset;
}

std::vector<SceneGraph*> SceneGraph::getChildren() {
  return children;
}

void SceneGraph::addSceneGraph(SceneGraph *sceneGraph) {
  children.push_back(sceneGraph);
}

void SceneGraph::setPrepareRotationMatrix(mat4 _prepareRotationMatrix) {
  prepareRotationMatrix = _prepareRotationMatrix;
}

void SceneGraph::setApplyRotationMatrix(mat4 _applyRotationMatrix) {
  applyRotationMatrix = _applyRotationMatrix;
}

void SceneGraph::setModelOffset(mat4 _modelOffset) {
  modelOffset = _modelOffset;
}
