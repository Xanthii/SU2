/*!
 * \file CTransfer_BoundaryDisplacements.cpp
 * \brief Main subroutines for transferring boundary displacements.
 * \author Ruben Sanchez
 * \version 6.2.0 "Falcon"
 *
 * The current SU2 release has been coordinated by the
 * SU2 International Developers Society <www.su2devsociety.org>
 * with selected contributions from the open-source community.
 *
 * The main research teams contributing to the current release are:
 *  - Prof. Juan J. Alonso's group at Stanford University.
 *  - Prof. Piero Colonna's group at Delft University of Technology.
 *  - Prof. Nicolas R. Gauger's group at Kaiserslautern University of Technology.
 *  - Prof. Alberto Guardone's group at Polytechnic University of Milan.
 *  - Prof. Rafael Palacios' group at Imperial College London.
 *  - Prof. Vincent Terrapon's group at the University of Liege.
 *  - Prof. Edwin van der Weide's group at the University of Twente.
 *  - Lab. of New Concepts in Aeronautics at Tech. Institute of Aeronautics.
 *
 * Copyright 2012-2019, Francisco D. Palacios, Thomas D. Economon,
 *                      Tim Albring, and the SU2 contributors.
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../../include/transfer/physics/CTransfer_BoundaryDisplacements.hpp"

CTransfer_BoundaryDisplacements::CTransfer_BoundaryDisplacements(unsigned short val_nVar,
                                                                 unsigned short val_nConst,
                                                                 CConfig *config) :
                                                                 CTransfer(val_nVar, val_nConst, config) {

}

CTransfer_BoundaryDisplacements::~CTransfer_BoundaryDisplacements(void) {

}


void CTransfer_BoundaryDisplacements::GetPhysical_Constants(CSolver *struct_solution, CSolver *flow_solution,
                                                              CGeometry *struct_geometry, CGeometry *flow_geometry,
                                                              CConfig *struct_config, CConfig *flow_config) {
}

void CTransfer_BoundaryDisplacements::GetDonor_Variable(CSolver *struct_solution, CGeometry *struct_geometry,
                                                        CConfig *struct_config, unsigned long Marker_Struct,
                                                        unsigned long Vertex_Struct, unsigned long Point_Struct) {

  su2double *DisplacementDonor;
  unsigned short iVar;

  /*--- The displacements come from the predicted solution, but they are no longer incremental ---*/
  DisplacementDonor = struct_solution->node[Point_Struct]->GetSolution_Pred();

  for (iVar = 0; iVar < nVar; iVar++)
    Donor_Variable[iVar] = DisplacementDonor[iVar];

}

void CTransfer_BoundaryDisplacements::SetTarget_Variable(CSolver *mesh_solver, CGeometry *flow_geometry,
                               CConfig *flow_config, unsigned long Marker_Flow,
                               unsigned long Vertex_Flow, unsigned long Point_Mesh) {

  /*--- Impose the boundary displacements ---*/

  mesh_solver->node[Point_Mesh]->SetBound_Disp(Target_Variable);

}