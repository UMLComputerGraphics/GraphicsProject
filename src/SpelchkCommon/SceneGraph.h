/*
 * SceneGraph.h
 *
 *  Created on: Dec 11, 2012
 *      Author: soujiroboi
 */

#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

#include "Model.h"

class SceneGraph {
  private:
    Model *model;
    mat4 prepareRotationMatrix;         // Move model to pivot point
    mat4 applyRotationMatrix;           // Apply rotation around pivot point
    mat4 modelOffset;                   // Moves model to its final location relative to parent and pivot point
    std::vector<SceneGraph*> children;

  public:
    SceneGraph(Model *model, mat4 prepareRotationMatrix, mat4 modelOffset);
    virtual ~SceneGraph();

    Model* getModel();
    mat4 getPrepareRotationMatrix();
    mat4 getApplyRotationMatrix();
    mat4 getModelOffset();
    std::vector<SceneGraph*> getChildren();

    void addSceneGraph(SceneGraph *sceneGraph);

    void setPrepareRotationMatrix(mat4 prepareRotationMatrix);
    void setApplyRotationMatrix(mat4 applyRotationMatrix);
    void setModelOffset(mat4 modelOffset);
};

#endif /* SCENEGRAPH_H_ */
