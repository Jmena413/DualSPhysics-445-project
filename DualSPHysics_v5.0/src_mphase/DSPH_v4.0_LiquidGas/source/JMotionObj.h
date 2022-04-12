//HEAD_DSCODES
/*
<DUALSPHYSICS>  Copyright (c) 2018 by Dr Jose M. Dominguez et al. (see http://dual.sphysics.org/index.php/developers/). 

EPHYSLAB Environmental Physics Laboratory, Universidade de Vigo, Ourense, Spain.
School of Mechanical, Aerospace and Civil Engineering, University of Manchester, Manchester, U.K.

This file is part of DualSPHysics. 

DualSPHysics is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License 
as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

DualSPHysics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details. 

You should have received a copy of the GNU Lesser General Public License along with DualSPHysics. If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef _JMotionObj_
#define _JMotionObj_

#include "TypesDef.h"
#include "JObject.h"
#include "JMotionPos.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

class JMotionMov;
class JMotionAxis;
class TiXmlNode;


//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovActive : protected JObject
{
public:
  const double EventFinish;
  const std::string* Dirin;
  double Start,Finish;
  bool Flash;
  tdouble3 Vel;    //-Solo se usa para el RectilinearAce
  double VelAng;   //-Solo se usa para el RotationAce
  tdouble3 Phase;  //-Solo se usa para el RectilinearSinusoidal
  double PhaseUni; //-Solo se usa para el RotationSinusoidal y CircularSinusoidal

  //-Vars para el MovRectFile y MovRotFile
  static const unsigned DFSIZEMAX=104857600; ///<Maximum file size (100mb).
  bool DfPosType;    //-Indica que se almacenan posiciones.
  unsigned DfSize;   //-Posiciones reservadas para vectores
  unsigned DfCount;  //-Numero de posiciones
  unsigned DfIndex;  //-Indice de posicionamiento temporal
  tdouble3 DfLastPos;
  double DfLastAng;
  double *DfTimes;   //-Tiempos
  tdouble3 *DfPos;   //-Posiciones
  double *DfAng;     //-Angulos, siemgre en grados.

  JMotionMov* Mov;
  bool Del;

  JMotionMovActive(double start,double eventfinish,JMotionMov* mov,const std::string* dirin);
  ~JMotionMovActive();

  void ConfigData();
  void NextMov();

  void DfReset();
  void DfConfig(bool postype);
  void DfResize(unsigned size);
  void DfLoadFilePos(std::string file,const int fields,const int fieldtime,const int fieldx,const int fieldy,const int fieldz);
  void DfLoadFileAng(std::string file,bool angdegrees);

  tdouble3 DfGetNewPos(double t);
  double DfGetNewAng(double t);
};


class JMotion;
class JMotionEvent;
//==============================================================================
//##############################################################################
//==============================================================================
class JMotionObj : protected JObject
{
private:
  const std::string* Dirin;
  //JMotionPos Pos; //-Solo mantiene el desplazamiento acumulado cuando tiene hijos q puedan usarlo.
  JMotionPos ModPos;
  bool Moving;
  std::vector<JMotionObj*> Children;  //-Objetos hijos.
  std::vector<JMotionMov*> Movs;      //-Movimientos asociados
  std::vector<JMotionAxis*> Axis;     //-Ejes asociados
  std::vector<JMotionEvent*> Events;  //-Eventos asociados
  std::vector<JMotionMovActive*> ActiveMovs;  //-Movimientos activos
  
  int GetPosMov(JMotionMov* mv)const;
  void WriteXml(TiXmlNode* node,const JMotionEvent &evt)const;
public:
  const unsigned Id; //-Identificador de objeto
  const int Ref;     //-Referencia a objeto real (ref<0 lo trata como un obj virtual)
  const JMotionObj* Parent;
  bool Active;

  JMotionObj(unsigned id,JMotionObj* parent,int ref,const std::string* dirin);
  ~JMotionObj();
  void Reset();
  JMotionObj* ObjGetPointer(unsigned id);
  JMotionObj* ObjGetPointerByRef(int ref);
  JMotionMov* MovGetPointer(unsigned id)const;
  JMotionAxis* AxisGetPointer(const tdouble3 &p1,const tdouble3 &p2)const;
  void AddChild(JMotionObj* obj);
  void AddMov(JMotionMov* mov);
  void AddAxis(JMotionAxis* axis);
  void AddEvent(JMotionEvent* evt);
  bool LinkMovs();
  unsigned ChildrenCount();
  void BeginEvent(double start,double eventfinish,JMotionMov* mov);
 
  bool ProcesTime(double timestep,double dt,JMotionObj** lismov,unsigned &lismovcount,JMotionObj** lisstop,unsigned &lisstopcount);
  bool GetMov(unsigned &ref,tdouble3 &mvsimple,JMatrix4d &mvmatrix)const;
  int GetMaxRef()const;

  void CopyConfigMovs(JMotion &mot)const;
  void CopyConfig(JMotion &mot)const;
  void CopyChangeRef(JMotion &mot,const int* ref,const int* refnew,unsigned refcount)const;
  bool ExistsObj(JMotionObj* obj)const;
  bool Optimize();

  void WriteXml(TiXmlNode* node)const;
};


#endif

