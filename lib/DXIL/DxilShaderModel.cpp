///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// DxilShaderModel.cpp                                                       //
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
// This file is distributed under the University of Illinois Open Source     //
// License. See LICENSE.TXT for details.                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <limits.h>

#include "dxc/DXIL/DxilSemantic.h"
#include "dxc/DXIL/DxilShaderModel.h"
#include "dxc/Support/Global.h"

#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/ErrorHandling.h"

#include <algorithm>

namespace hlsl {

ShaderModel::ShaderModel(Kind Kind, unsigned Major, unsigned Minor,
                         const char *pszName, unsigned NumInputRegs,
                         unsigned NumOutputRegs, bool bUAVs, bool bTypedUavs,
                         unsigned NumUAVRegs)
    : m_Kind(Kind), m_Major(Major), m_Minor(Minor), m_pszName(pszName),
      m_NumInputRegs(NumInputRegs), m_NumOutputRegs(NumOutputRegs),
      m_bTypedUavs(bTypedUavs), m_NumUAVRegs(NumUAVRegs) {}

bool ShaderModel::operator==(const ShaderModel &other) const {
  return m_Kind == other.m_Kind && m_Major == other.m_Major &&
         m_Minor == other.m_Minor && strcmp(m_pszName, other.m_pszName) == 0 &&
         m_NumInputRegs == other.m_NumInputRegs &&
         m_NumOutputRegs == other.m_NumOutputRegs &&
         m_bTypedUavs == other.m_bTypedUavs &&
         m_NumUAVRegs == other.m_NumUAVRegs;
}

bool ShaderModel::IsValid() const {
  DXASSERT(IsPS() || IsVS() || IsGS() || IsHS() || IsDS() || IsCS() ||
               IsLib() || IsMS() || IsAS() || m_Kind == Kind::Invalid,
           "invalid shader model");
  return m_Kind != Kind::Invalid;
}

bool ShaderModel::IsValidForDxil() const {
  if (!IsValid())
    return false;
  switch (m_Major) {
  case 6: {
    switch (m_Minor) {
      // clang-format off
      // Python lines need to be not formatted.
      /* <py::lines('VALRULE-TEXT')>hctdb_instrhelp.get_is_valid_for_dxil()</py>*/
    // clang-format on
    // VALRULE-TEXT:BEGIN
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      // VALRULE-TEXT:END
      return true;
    case kOfflineMinor:
      return m_Kind == Kind::Library;
    }
  } break;
  }
  return false;
}

const ShaderModel *ShaderModel::Get(Kind Kind, unsigned Major, unsigned Minor) {
  /* <py::lines('VALRULE-TEXT')>hctdb_instrhelp.get_shader_model_get()</py>*/
  // VALRULE-TEXT:BEGIN
  const static std::pair<unsigned, unsigned> hashToIdxMap[] = {
      {1024, 0},    // ps_4_0
      {1025, 1},    // ps_4_1
      {1280, 2},    // ps_5_0
      {1281, 3},    // ps_5_1
      {1536, 4},    // ps_6_0
      {1537, 5},    // ps_6_1
      {1538, 6},    // ps_6_2
      {1539, 7},    // ps_6_3
      {1540, 8},    // ps_6_4
      {1541, 9},    // ps_6_5
      {1542, 10},   // ps_6_6
      {1543, 11},   // ps_6_7
      {1544, 12},   // ps_6_8
      {66560, 13},  // vs_4_0
      {66561, 14},  // vs_4_1
      {66816, 15},  // vs_5_0
      {66817, 16},  // vs_5_1
      {67072, 17},  // vs_6_0
      {67073, 18},  // vs_6_1
      {67074, 19},  // vs_6_2
      {67075, 20},  // vs_6_3
      {67076, 21},  // vs_6_4
      {67077, 22},  // vs_6_5
      {67078, 23},  // vs_6_6
      {67079, 24},  // vs_6_7
      {67080, 25},  // vs_6_8
      {132096, 26}, // gs_4_0
      {132097, 27}, // gs_4_1
      {132352, 28}, // gs_5_0
      {132353, 29}, // gs_5_1
      {132608, 30}, // gs_6_0
      {132609, 31}, // gs_6_1
      {132610, 32}, // gs_6_2
      {132611, 33}, // gs_6_3
      {132612, 34}, // gs_6_4
      {132613, 35}, // gs_6_5
      {132614, 36}, // gs_6_6
      {132615, 37}, // gs_6_7
      {132616, 38}, // gs_6_8
      {197888, 39}, // hs_5_0
      {197889, 40}, // hs_5_1
      {198144, 41}, // hs_6_0
      {198145, 42}, // hs_6_1
      {198146, 43}, // hs_6_2
      {198147, 44}, // hs_6_3
      {198148, 45}, // hs_6_4
      {198149, 46}, // hs_6_5
      {198150, 47}, // hs_6_6
      {198151, 48}, // hs_6_7
      {198152, 49}, // hs_6_8
      {263424, 50}, // ds_5_0
      {263425, 51}, // ds_5_1
      {263680, 52}, // ds_6_0
      {263681, 53}, // ds_6_1
      {263682, 54}, // ds_6_2
      {263683, 55}, // ds_6_3
      {263684, 56}, // ds_6_4
      {263685, 57}, // ds_6_5
      {263686, 58}, // ds_6_6
      {263687, 59}, // ds_6_7
      {263688, 60}, // ds_6_8
      {328704, 61}, // cs_4_0
      {328705, 62}, // cs_4_1
      {328960, 63}, // cs_5_0
      {328961, 64}, // cs_5_1
      {329216, 65}, // cs_6_0
      {329217, 66}, // cs_6_1
      {329218, 67}, // cs_6_2
      {329219, 68}, // cs_6_3
      {329220, 69}, // cs_6_4
      {329221, 70}, // cs_6_5
      {329222, 71}, // cs_6_6
      {329223, 72}, // cs_6_7
      {329224, 73}, // cs_6_8
      {394753, 74}, // lib_6_1
      {394754, 75}, // lib_6_2
      {394755, 76}, // lib_6_3
      {394756, 77}, // lib_6_4
      {394757, 78}, // lib_6_5
      {394758, 79}, // lib_6_6
      {394759, 80}, // lib_6_7
      {394760, 81}, // lib_6_8
      // lib_6_x is for offline linking only, and relaxes restrictions
      {394767, 82}, // lib_6_x
      {853509, 83}, // ms_6_5
      {853510, 84}, // ms_6_6
      {853511, 85}, // ms_6_7
      {853512, 86}, // ms_6_8
      {919045, 87}, // as_6_5
      {919046, 88}, // as_6_6
      {919047, 89}, // as_6_7
      {919048, 90}, // as_6_8
  };
  unsigned hash = (unsigned)Kind << 16 | Major << 8 | Minor;
  auto pred = [](const std::pair<unsigned, unsigned> &elem, unsigned val) {
    return elem.first < val;
  };
  auto it = std::lower_bound(std::begin(hashToIdxMap), std::end(hashToIdxMap),
                             hash, pred);
  if (it == std::end(hashToIdxMap) || it->first != hash)
    return GetInvalid();
  return &ms_ShaderModels[it->second];
  // VALRULE-TEXT:END
}

const ShaderModel *ShaderModel::GetByName(llvm::StringRef Name) {
  // [ps|vs|gs|hs|ds|cs|ms|as]_[major]_[minor]
  Kind kind;
  if (Name.empty()) {
    return GetInvalid();
  }

  switch (Name[0]) {
  case 'p':
    kind = Kind::Pixel;
    break;
  case 'v':
    kind = Kind::Vertex;
    break;
  case 'g':
    kind = Kind::Geometry;
    break;
  case 'h':
    kind = Kind::Hull;
    break;
  case 'd':
    kind = Kind::Domain;
    break;
  case 'c':
    kind = Kind::Compute;
    break;
  case 'l':
    kind = Kind::Library;
    break;
  case 'm':
    kind = Kind::Mesh;
    break;
  case 'a':
    kind = Kind::Amplification;
    break;
  default:
    return GetInvalid();
  }
  unsigned Idx = 3;
  if (kind != Kind::Library) {
    if (Name[1] != 's' || Name[2] != '_')
      return GetInvalid();
  } else {
    if (Name[1] != 'i' || Name[2] != 'b' || Name[3] != '_')
      return GetInvalid();
    Idx = 4;
  }

  unsigned Major;
  switch (Name[Idx++]) {
  case '4':
    Major = 4;
    break;
  case '5':
    Major = 5;
    break;
  case '6':
    Major = 6;
    break;
  default:
    return GetInvalid();
  }
  if (Name[Idx++] != '_')
    return GetInvalid();

  unsigned Minor;
  switch (Name[Idx++]) {
  case '0':
    Minor = 0;
    break;
  case '1':
    Minor = 1;
    break;
    // clang-format off
  // Python lines need to be not formatted.
  /* <py::lines('VALRULE-TEXT')>hctdb_instrhelp.get_shader_model_by_name()</py>*/
  // clang-format on
  // VALRULE-TEXT:BEGIN
  case '2':
    if (Major == 6) {
      Minor = 2;
      break;
    } else
      return GetInvalid();
  case '3':
    if (Major == 6) {
      Minor = 3;
      break;
    } else
      return GetInvalid();
  case '4':
    if (Major == 6) {
      Minor = 4;
      break;
    } else
      return GetInvalid();
  case '5':
    if (Major == 6) {
      Minor = 5;
      break;
    } else
      return GetInvalid();
  case '6':
    if (Major == 6) {
      Minor = 6;
      break;
    } else
      return GetInvalid();
  case '7':
    if (Major == 6) {
      Minor = 7;
      break;
    } else
      return GetInvalid();
  case '8':
    if (Major == 6) {
      Minor = 8;
      break;
    } else
      return GetInvalid();
    // VALRULE-TEXT:END
  case 'x':
    if (kind == Kind::Library && Major == 6) {
      Minor = kOfflineMinor;
      break;
    } else
      return GetInvalid();
  default:
    return GetInvalid();
  }
  // make sure there isn't anything after the minor
  if (Name.size() > Idx)
    return GetInvalid();

  return Get(kind, Major, Minor);
}

void ShaderModel::GetDxilVersion(unsigned &DxilMajor,
                                 unsigned &DxilMinor) const {
  DXASSERT(IsValidForDxil(), "invalid shader model");
  DxilMajor = 1;
  switch (m_Minor) {
  /* <py::lines('VALRULE-TEXT')>hctdb_instrhelp.get_dxil_version()</py>*/
  // VALRULE-TEXT:BEGIN
  case 0:
    DxilMinor = 0;
    break;
  case 1:
    DxilMinor = 1;
    break;
  case 2:
    DxilMinor = 2;
    break;
  case 3:
    DxilMinor = 3;
    break;
  case 4:
    DxilMinor = 4;
    break;
  case 5:
    DxilMinor = 5;
    break;
  case 6:
    DxilMinor = 6;
    break;
  case 7:
    DxilMinor = 7;
    break;
  case 8:
    DxilMinor = 8;
    break;
  case kOfflineMinor: // Always update this to highest dxil version
    DxilMinor = 8;
    break;
  // VALRULE-TEXT:END
  default:
    DXASSERT(0, "IsValidForDxil() should have caught this.");
    break;
  }
}

void ShaderModel::GetMinValidatorVersion(unsigned &ValMajor,
                                         unsigned &ValMinor) const {
  DXASSERT(IsValidForDxil(), "invalid shader model");
  ValMajor = 1;
  switch (m_Minor) {
    // clang-format off
  // Python lines need to be not formatted.
  /* <py::lines('VALRULE-TEXT')>hctdb_instrhelp.get_min_validator_version()</py>*/
  // clang-format on
  // VALRULE-TEXT:BEGIN
  case 0:
    ValMinor = 0;
    break;
  case 1:
    ValMinor = 1;
    break;
  case 2:
    ValMinor = 2;
    break;
  case 3:
    ValMinor = 3;
    break;
  case 4:
    ValMinor = 4;
    break;
  case 5:
    ValMinor = 5;
    break;
  case 6:
    ValMinor = 6;
    break;
  case 7:
    ValMinor = 7;
    break;
  case 8:
    ValMinor = 8;
    break;
  // VALRULE-TEXT:END
  case kOfflineMinor:
    ValMajor = 0;
    ValMinor = 0;
    break;
  default:
    DXASSERT(0, "IsValidForDxil() should have caught this.");
    break;
  }
}

static const char *ShaderModelKindNames[] = {
    "ps",           "vs",     "gs",         "hs",
    "ds",           "cs",     "lib",        "raygeneration",
    "intersection", "anyhit", "closesthit", "miss",
    "callable",     "ms",     "as",         "node",
    "invalid",
};

const char *ShaderModel::GetKindName() const { return GetKindName(m_Kind); }

const char *ShaderModel::GetKindName(Kind kind) {
  static_assert(static_cast<unsigned>(Kind::Invalid) ==
                    _countof(ShaderModelKindNames) - 1,
                "Invalid kinds or names");
  return ShaderModelKindNames[static_cast<unsigned int>(kind)];
}

const ShaderModel *ShaderModel::GetInvalid() {
  return &ms_ShaderModels[kNumShaderModels - 1];
}

DXIL::ShaderKind ShaderModel::KindFromFullName(llvm::StringRef Name) {
  return llvm::StringSwitch<DXIL::ShaderKind>(Name)
      .Case("pixel", DXIL::ShaderKind::Pixel)
      .Case("vertex", DXIL::ShaderKind::Vertex)
      .Case("geometry", DXIL::ShaderKind::Geometry)
      .Case("hull", DXIL::ShaderKind::Hull)
      .Case("domain", DXIL::ShaderKind::Domain)
      .Case("compute", DXIL::ShaderKind::Compute)
      .Case("raygeneration", DXIL::ShaderKind::RayGeneration)
      .Case("intersection", DXIL::ShaderKind::Intersection)
      .Case("anyhit", DXIL::ShaderKind::AnyHit)
      .Case("closesthit", DXIL::ShaderKind::ClosestHit)
      .Case("miss", DXIL::ShaderKind::Miss)
      .Case("callable", DXIL::ShaderKind::Callable)
      .Case("mesh", DXIL::ShaderKind::Mesh)
      .Case("amplification", DXIL::ShaderKind::Amplification)
      .Case("node", DXIL::ShaderKind::Node)
      .Default(DXIL::ShaderKind::Invalid);
}

const llvm::StringRef ShaderModel::FullNameFromKind(DXIL::ShaderKind sk) {
  switch (sk) {
  case DXIL::ShaderKind::Pixel:
    return "pixel";
  case DXIL::ShaderKind::Vertex:
    return "vertex";
  case DXIL::ShaderKind::Geometry:
    return "geometry";
  case DXIL::ShaderKind::Hull:
    return "hull";
  case DXIL::ShaderKind::Domain:
    return "domain";
  case DXIL::ShaderKind::Compute:
    return "compute";
  // Library has no full name for use with shader attribute.
  case DXIL::ShaderKind::Library:
  case DXIL::ShaderKind::Invalid:
    return llvm::StringRef();
  case DXIL::ShaderKind::RayGeneration:
    return "raygeneration";
  case DXIL::ShaderKind::Intersection:
    return "intersection";
  case DXIL::ShaderKind::AnyHit:
    return "anyhit";
  case DXIL::ShaderKind::ClosestHit:
    return "closesthit";
  case DXIL::ShaderKind::Miss:
    return "miss";
  case DXIL::ShaderKind::Callable:
    return "callable";
  case DXIL::ShaderKind::Mesh:
    return "mesh";
  case DXIL::ShaderKind::Amplification:
    return "amplification";
  case DXIL::ShaderKind::Node:
    return "node";
  default:
    llvm_unreachable("unknown ShaderKind");
  }
}

bool ShaderModel::AllowDerivatives(DXIL::ShaderKind sk) const {
  switch (sk) {
  case DXIL::ShaderKind::Pixel:
  case DXIL::ShaderKind::Library:
  case DXIL::ShaderKind::Node:
    return true;
  case DXIL::ShaderKind::Compute:
  case DXIL::ShaderKind::Amplification:
  case DXIL::ShaderKind::Mesh:
    return IsSM66Plus();
  }
  return false;
}

typedef ShaderModel SM;
typedef Semantic SE;
const ShaderModel ShaderModel::ms_ShaderModels[kNumShaderModels] = {
    //                                  IR  OR   UAV?   TyUAV? UAV base
    /* <py::lines('VALRULE-TEXT')>hctdb_instrhelp.get_shader_models()</py>*/
    // VALRULE-TEXT:BEGIN
    SM(Kind::Pixel, 4, 0, "ps_4_0", 32, 8, false, false, 0),
    SM(Kind::Pixel, 4, 1, "ps_4_1", 32, 8, false, false, 0),
    SM(Kind::Pixel, 5, 0, "ps_5_0", 32, 8, true, true, 64),
    SM(Kind::Pixel, 5, 1, "ps_5_1", 32, 8, true, true, 64),
    SM(Kind::Pixel, 6, 0, "ps_6_0", 32, 8, true, true, UINT_MAX),
    SM(Kind::Pixel, 6, 1, "ps_6_1", 32, 8, true, true, UINT_MAX),
    SM(Kind::Pixel, 6, 2, "ps_6_2", 32, 8, true, true, UINT_MAX),
    SM(Kind::Pixel, 6, 3, "ps_6_3", 32, 8, true, true, UINT_MAX),
    SM(Kind::Pixel, 6, 4, "ps_6_4", 32, 8, true, true, UINT_MAX),
    SM(Kind::Pixel, 6, 5, "ps_6_5", 32, 8, true, true, UINT_MAX),
    SM(Kind::Pixel, 6, 6, "ps_6_6", 32, 8, true, true, UINT_MAX),
    SM(Kind::Pixel, 6, 7, "ps_6_7", 32, 8, true, true, UINT_MAX),
    SM(Kind::Pixel, 6, 8, "ps_6_8", 32, 8, true, true, UINT_MAX),
    SM(Kind::Vertex, 4, 0, "vs_4_0", 16, 16, false, false, 0),
    SM(Kind::Vertex, 4, 1, "vs_4_1", 32, 32, false, false, 0),
    SM(Kind::Vertex, 5, 0, "vs_5_0", 32, 32, true, true, 64),
    SM(Kind::Vertex, 5, 1, "vs_5_1", 32, 32, true, true, 64),
    SM(Kind::Vertex, 6, 0, "vs_6_0", 32, 32, true, true, UINT_MAX),
    SM(Kind::Vertex, 6, 1, "vs_6_1", 32, 32, true, true, UINT_MAX),
    SM(Kind::Vertex, 6, 2, "vs_6_2", 32, 32, true, true, UINT_MAX),
    SM(Kind::Vertex, 6, 3, "vs_6_3", 32, 32, true, true, UINT_MAX),
    SM(Kind::Vertex, 6, 4, "vs_6_4", 32, 32, true, true, UINT_MAX),
    SM(Kind::Vertex, 6, 5, "vs_6_5", 32, 32, true, true, UINT_MAX),
    SM(Kind::Vertex, 6, 6, "vs_6_6", 32, 32, true, true, UINT_MAX),
    SM(Kind::Vertex, 6, 7, "vs_6_7", 32, 32, true, true, UINT_MAX),
    SM(Kind::Vertex, 6, 8, "vs_6_8", 32, 32, true, true, UINT_MAX),
    SM(Kind::Geometry, 4, 0, "gs_4_0", 16, 32, false, false, 0),
    SM(Kind::Geometry, 4, 1, "gs_4_1", 32, 32, false, false, 0),
    SM(Kind::Geometry, 5, 0, "gs_5_0", 32, 32, true, true, 64),
    SM(Kind::Geometry, 5, 1, "gs_5_1", 32, 32, true, true, 64),
    SM(Kind::Geometry, 6, 0, "gs_6_0", 32, 32, true, true, UINT_MAX),
    SM(Kind::Geometry, 6, 1, "gs_6_1", 32, 32, true, true, UINT_MAX),
    SM(Kind::Geometry, 6, 2, "gs_6_2", 32, 32, true, true, UINT_MAX),
    SM(Kind::Geometry, 6, 3, "gs_6_3", 32, 32, true, true, UINT_MAX),
    SM(Kind::Geometry, 6, 4, "gs_6_4", 32, 32, true, true, UINT_MAX),
    SM(Kind::Geometry, 6, 5, "gs_6_5", 32, 32, true, true, UINT_MAX),
    SM(Kind::Geometry, 6, 6, "gs_6_6", 32, 32, true, true, UINT_MAX),
    SM(Kind::Geometry, 6, 7, "gs_6_7", 32, 32, true, true, UINT_MAX),
    SM(Kind::Geometry, 6, 8, "gs_6_8", 32, 32, true, true, UINT_MAX),
    SM(Kind::Hull, 5, 0, "hs_5_0", 32, 32, true, true, 64),
    SM(Kind::Hull, 5, 1, "hs_5_1", 32, 32, true, true, 64),
    SM(Kind::Hull, 6, 0, "hs_6_0", 32, 32, true, true, UINT_MAX),
    SM(Kind::Hull, 6, 1, "hs_6_1", 32, 32, true, true, UINT_MAX),
    SM(Kind::Hull, 6, 2, "hs_6_2", 32, 32, true, true, UINT_MAX),
    SM(Kind::Hull, 6, 3, "hs_6_3", 32, 32, true, true, UINT_MAX),
    SM(Kind::Hull, 6, 4, "hs_6_4", 32, 32, true, true, UINT_MAX),
    SM(Kind::Hull, 6, 5, "hs_6_5", 32, 32, true, true, UINT_MAX),
    SM(Kind::Hull, 6, 6, "hs_6_6", 32, 32, true, true, UINT_MAX),
    SM(Kind::Hull, 6, 7, "hs_6_7", 32, 32, true, true, UINT_MAX),
    SM(Kind::Hull, 6, 8, "hs_6_8", 32, 32, true, true, UINT_MAX),
    SM(Kind::Domain, 5, 0, "ds_5_0", 32, 32, true, true, 64),
    SM(Kind::Domain, 5, 1, "ds_5_1", 32, 32, true, true, 64),
    SM(Kind::Domain, 6, 0, "ds_6_0", 32, 32, true, true, UINT_MAX),
    SM(Kind::Domain, 6, 1, "ds_6_1", 32, 32, true, true, UINT_MAX),
    SM(Kind::Domain, 6, 2, "ds_6_2", 32, 32, true, true, UINT_MAX),
    SM(Kind::Domain, 6, 3, "ds_6_3", 32, 32, true, true, UINT_MAX),
    SM(Kind::Domain, 6, 4, "ds_6_4", 32, 32, true, true, UINT_MAX),
    SM(Kind::Domain, 6, 5, "ds_6_5", 32, 32, true, true, UINT_MAX),
    SM(Kind::Domain, 6, 6, "ds_6_6", 32, 32, true, true, UINT_MAX),
    SM(Kind::Domain, 6, 7, "ds_6_7", 32, 32, true, true, UINT_MAX),
    SM(Kind::Domain, 6, 8, "ds_6_8", 32, 32, true, true, UINT_MAX),
    SM(Kind::Compute, 4, 0, "cs_4_0", 0, 0, false, false, 0),
    SM(Kind::Compute, 4, 1, "cs_4_1", 0, 0, false, false, 0),
    SM(Kind::Compute, 5, 0, "cs_5_0", 0, 0, true, true, 64),
    SM(Kind::Compute, 5, 1, "cs_5_1", 0, 0, true, true, 64),
    SM(Kind::Compute, 6, 0, "cs_6_0", 0, 0, true, true, UINT_MAX),
    SM(Kind::Compute, 6, 1, "cs_6_1", 0, 0, true, true, UINT_MAX),
    SM(Kind::Compute, 6, 2, "cs_6_2", 0, 0, true, true, UINT_MAX),
    SM(Kind::Compute, 6, 3, "cs_6_3", 0, 0, true, true, UINT_MAX),
    SM(Kind::Compute, 6, 4, "cs_6_4", 0, 0, true, true, UINT_MAX),
    SM(Kind::Compute, 6, 5, "cs_6_5", 0, 0, true, true, UINT_MAX),
    SM(Kind::Compute, 6, 6, "cs_6_6", 0, 0, true, true, UINT_MAX),
    SM(Kind::Compute, 6, 7, "cs_6_7", 0, 0, true, true, UINT_MAX),
    SM(Kind::Compute, 6, 8, "cs_6_8", 0, 0, true, true, UINT_MAX),
    SM(Kind::Library, 6, 1, "lib_6_1", 32, 32, true, true, UINT_MAX),
    SM(Kind::Library, 6, 2, "lib_6_2", 32, 32, true, true, UINT_MAX),
    SM(Kind::Library, 6, 3, "lib_6_3", 32, 32, true, true, UINT_MAX),
    SM(Kind::Library, 6, 4, "lib_6_4", 32, 32, true, true, UINT_MAX),
    SM(Kind::Library, 6, 5, "lib_6_5", 32, 32, true, true, UINT_MAX),
    SM(Kind::Library, 6, 6, "lib_6_6", 32, 32, true, true, UINT_MAX),
    SM(Kind::Library, 6, 7, "lib_6_7", 32, 32, true, true, UINT_MAX),
    SM(Kind::Library, 6, 8, "lib_6_8", 32, 32, true, true, UINT_MAX),
    // lib_6_x is for offline linking only, and relaxes restrictions
    SM(Kind::Library, 6, kOfflineMinor, "lib_6_x", 32, 32, true, true,
       UINT_MAX),
    SM(Kind::Mesh, 6, 5, "ms_6_5", 0, 0, true, true, UINT_MAX),
    SM(Kind::Mesh, 6, 6, "ms_6_6", 0, 0, true, true, UINT_MAX),
    SM(Kind::Mesh, 6, 7, "ms_6_7", 0, 0, true, true, UINT_MAX),
    SM(Kind::Mesh, 6, 8, "ms_6_8", 0, 0, true, true, UINT_MAX),
    SM(Kind::Amplification, 6, 5, "as_6_5", 0, 0, true, true, UINT_MAX),
    SM(Kind::Amplification, 6, 6, "as_6_6", 0, 0, true, true, UINT_MAX),
    SM(Kind::Amplification, 6, 7, "as_6_7", 0, 0, true, true, UINT_MAX),
    SM(Kind::Amplification, 6, 8, "as_6_8", 0, 0, true, true, UINT_MAX),
    // Values before Invalid must remain sorted by Kind, then Major, then Minor.
    SM(Kind::Invalid, 0, 0, "invalid", 0, 0, false, false, 0),
    // VALRULE-TEXT:END
};

static const char *NodeLaunchTypeNames[] = {"invalid", "broadcasting",
                                            "coalescing", "thread"};

const char *ShaderModel::GetNodeLaunchTypeName(DXIL::NodeLaunchType launchTy) {
  static_assert(static_cast<unsigned>(DXIL::NodeLaunchType::Thread) ==
                    _countof(NodeLaunchTypeNames) - 1,
                "Invalid launch type or names");
  return NodeLaunchTypeNames[static_cast<unsigned int>(launchTy)];
}

DXIL::NodeLaunchType ShaderModel::NodeLaunchTypeFromName(llvm::StringRef name) {
  return llvm::StringSwitch<DXIL::NodeLaunchType>(name.lower())
      .Case("broadcasting", DXIL::NodeLaunchType::Broadcasting)
      .Case("coalescing", DXIL::NodeLaunchType::Coalescing)
      .Case("thread", DXIL::NodeLaunchType::Thread)
      .Default(DXIL::NodeLaunchType::Invalid);
}

} // namespace hlsl
