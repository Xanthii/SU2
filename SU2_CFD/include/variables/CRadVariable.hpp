/*!
 * \file CRadVariable.hpp
 * \brief Class for defining the variables of the radiation solver.
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
 
#pragma once

#include "CVariable.hpp"

class CRadVariable : public CVariable {

private:

protected:

  MatrixType Radiative_SourceTerm;

public:

  /*!
   * \brief Constructor of the class.
   * \param[in] npoint  - Number of points in the problem.
   * \param[in] ndim - Number of dimensions of the problem.
   * \param[in] nvar - Number of variables of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  CRadVariable(unsigned long npoint, unsigned long ndim, unsigned long nvar, CConfig *config);

  /*!
   * \brief Destructor of the class.
   */
  ~CRadVariable(void);

  /*!
   * \brief Get the Radiative source term at the node
   * \return Radiative source term for the energy equation
   */
  inline su2double *GetRadiative_SourceTerm(unsigned long iPoint) { return Radiative_SourceTerm[iPoint];}

  /*!
   * \brief Set the Radiative source term at the node
   * \param[in] val_RadSourceTerm - value of the radiative source term
   */
  inline void SetRadiative_SourceTerm(unsigned long iPoint, unsigned long iVar, su2double val_RadSourceTerm) { Radiative_SourceTerm(iPoint, iVar) = val_RadSourceTerm;}


};