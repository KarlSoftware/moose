/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef MULTIAPPNEARESTNODETRANSFER_H
#define MULTIAPPNEARESTNODETRANSFER_H

#include "MultiAppTransfer.h"

class MooseVariable;
class MultiAppNearestNodeTransfer;

template<>
InputParameters validParams<MultiAppNearestNodeTransfer>();

/**
 * Copy the value to the target domain from the nearest node in the source domain.
 */
class MultiAppNearestNodeTransfer :
  public MultiAppTransfer
{
public:
  MultiAppNearestNodeTransfer(const std::string & name, InputParameters parameters);
  virtual ~MultiAppNearestNodeTransfer() {}

  virtual void initialSetup();

  virtual void execute();

protected:
  virtual void transferToMultiApp();
  virtual void transferFromMultiApp();

  /**
   * Return the nearest node to the point p.
   * @param p The point you want to find the nearest node to.
   * @param distance This will hold the distance between the returned node and p
   * @param mesh The mesh in which we search for the node
   * @param local true if we look at local nodes, otherwise we look at all nodes
   * @return The Node closest to point p.
   */
  Node * getNearestNode(const Point & p, Real & distance, MooseMesh * mesh, bool local);

  /**
   * Return the distance between the given point and the farthest corner of the
   * given bounding box.
   * @param p The point to evaluate all distances from.
   * @param bbox The bounding box to evaluate the distance to.
   * @return The maximum distance between the point p and the eight corners of
   * the bounding box bbox.
   */
  Real bboxMaxDistance(Point p, MeshTools::BoundingBox bbox);

  /**
   * Return the distance between the given point and the nearest corner of the
   * given bounding box.
   * @param p The point to evaluate all distances from.
   * @param bbox The bounding box to evaluate the distance to.
   * @return The minimum distance between the point p and the eight corners of
   * the bounding box bbox.
   */
  Real bboxMinDistance(Point p, MeshTools::BoundingBox bbox);

  std::vector<Node *> getLocalNodes(MooseMesh * mesh);

  AuxVariableName _to_var_name;
  VariableName _from_var_name;

/*
  bool _displaced_source_mesh;
  bool _displaced_target_mesh;
*/

  /// If true then node connections will be cached
  bool _fixed_meshes;

  /// Used to cache nodes
  std::map<dof_id_type, Node *> _node_map;

  /// Used to cache distances
  std::map<dof_id_type, Real> _distance_map;
};

#endif /* MULTIAPPVARIABLEVALUESAMPLEPOSTPROCESSORTRANSFER_H */
