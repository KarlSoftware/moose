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

#ifndef MULTIAPPTRANSFER_H
#define MULTIAPPTRANSFER_H

#include "Transfer.h"
#include "MultiApp.h"
#include "MooseEnum.h"
#include "MooseTypes.h"

class MultiAppTransfer;

template<>
InputParameters validParams<MultiAppTransfer>();

/**
 * Base class for all MultiAppTransfer objects.
 *
 * MultiAppTransfers are objects that push and pull values to and from MultiApp objects
 * from and to the main (master) system.
 *
 * Classes that inherit from this class still need to override the execute() method from Transfer.
 */
class MultiAppTransfer : public Transfer
{
public:
  MultiAppTransfer(const std::string & name, InputParameters parameters);
  virtual ~MultiAppTransfer() {}

  enum
  {
    TO_MULTIAPP,
    FROM_MULTIAPP
  };

  /// Used to construct InputParameters
  static MooseEnum directions() { return MooseEnum("to_multiapp from_multiapp"); }

  /// The direction this Transfer is going in
  int direction() { return _direction; }

  /**
   * Utility to verify that the vEariable in the destination system exists.
   */
  void variableIntegrityCheck(const AuxVariableName & var_name) const;

  /// Return the MultiApp that this transfer belongs to
  const MultiApp * getMultiApp() const { return _multi_app; }

protected:
  /// The MultiApp this Transfer is transferring data to or from
  MultiApp * _multi_app;

  /// Whether we're transferring to or from the MultiApp
  MooseEnum _direction;

  void getAppInfo();
  std::vector<processor_id_type> getFromProblem2ProcMap();
  NumericVector<Real> & getTransferVector(unsigned int i_local, std::string var_name);

  std::vector<FEProblem *> _to_problems;
  std::vector<FEProblem *> _from_problems;
  std::vector<EquationSystems *> _to_es;
  std::vector<EquationSystems *> _from_es;
  std::vector<MooseMesh *> _to_meshes;
  std::vector<MooseMesh *> _from_meshes;
  std::vector<Point> _to_positions;
  std::vector<Point> _from_positions;

  bool _displaced_source_mesh;
  bool _displaced_target_mesh;

  //bool _is_block_restricted = false;
  //bool _is_side_restricted = false;

  std::vector<MeshTools::BoundingBox> getBboxes();
  //std::vector<unsigned int> get_blocks();
  //std::vector<unsigned int> get_sides();

private:
  std::vector<unsigned int> _local2global_map;
};

#endif /* MULTIAPPTRANSFER_H */
