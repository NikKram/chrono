// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Daniel Melanz, Radu Serban
// =============================================================================
//
// Base class for a solid axle suspension modeled with bodies and constraints.
// Derived from ChSuspension, but still an abstract base class.
//
// The suspension subsystem is modeled with respect to a right-handed frame,
// with X pointing towards the front, Y to the left, and Z up (ISO standard).
// The suspension reference frame is assumed to be always aligned with that of
// the vehicle.  When attached to a chassis, only an offset is provided.
//
// All point locations are assumed to be given for the left half of the
// supspension and will be mirrored (reflecting the y coordinates) to construct
// the right side.
//
// =============================================================================

#ifndef CH_SOLIDAXLE_H
#define CH_SOLIDAXLE_H

#include <vector>

#include "chrono/assets/ChColorAsset.h"

#include "chrono_vehicle/ChApiVehicle.h"
#include "chrono_vehicle/wheeled_vehicle/ChSuspension.h"

namespace chrono {
namespace vehicle {

///
/// Base class for a solid axle suspension modeled with bodies and constraints.
/// Derived from ChSuspension, but still an abstract base class.
///
/// The suspension subsystem is modeled with respect to a right-handed frame,
/// with X pointing towards the front, Y to the left, and Z up (ISO standard).
/// The suspension reference frame is assumed to be always aligned with that of
/// the vehicle.  When attached to a chassis, only an offset is provided.
///
/// All point locations are assumed to be given for the left half of the
/// supspension and will be mirrored (reflecting the y coordinates) to construct
/// the right side.
///
class CH_VEHICLE_API ChSolidAxle : public ChSuspension {
  public:
    ChSolidAxle(const std::string& name  ///< [in] name of the subsystem
                );

    virtual ~ChSolidAxle() {}

    /// Specify whether or not this suspension can be steered.
    virtual bool IsSteerable() const { return true; }

    /// Specify whether or not this is an independent suspension.
    virtual bool IsIndependent() const override { return false; }

    /// Initialize this suspension subsystem.
    /// The suspension subsystem is initialized by attaching it to the specified
    /// chassis body at the specified location (with respect to and expressed in
    /// the reference frame of the chassis). It is assumed that the suspension
    /// reference frame is always aligned with the chassis reference frame.
    /// Finally, tierod_body is a handle to the body to which the suspension
    /// tierods are to be attached. For a steerable suspension, this will be the
    /// steering link of a suspension subsystem.  Otherwise, this is the chassis.
    virtual void Initialize(ChSharedPtr<ChBodyAuxRef> chassis,  ///< [in] handle to the chassis body
                            const ChVector<>& location,         ///< [in] location relative to the chassis frame
                            ChSharedPtr<ChBody> tierod_body     ///< [in] body to which tireods are connected
                            ) override;

    /// Get the force in the spring element.
    double GetSpringForce(VehicleSide side) const { return m_spring[side]->Get_SpringReact(); }

    /// Get the current length of the spring element
    double GetSpringLength(VehicleSide side) const { return m_spring[side]->Get_SpringLength(); }

    /// Get the current deformation of the spring element.
    double GetSpringDeformation(VehicleSide side) const { return m_spring[side]->Get_SpringDeform(); }

    /// Get the force in the shock (damper) element.
    double GetShockForce(VehicleSide side) const { return m_shock[side]->Get_SpringReact(); }

    /// Get the current length of the shock (damper) element.
    double GetShockLength(VehicleSide side) const { return m_shock[side]->Get_SpringLength(); }

    /// Get the current deformation velocity of the shock (damper) element.
    double GetShockVelocity(VehicleSide side) const { return m_shock[side]->Get_SpringVelocity(); }

    /// Log current constraint violations.
    virtual void LogConstraintViolations(VehicleSide side) override;

    void LogHardpointLocations(const ChVector<>& ref, bool inches = false);

  protected:
    /// Identifiers for the various hardpoints.
    enum PointId {
        SHOCK_A,     ///< shock, axle
        SHOCK_C,     ///< shock, chassis
        KNUCKLE_L,   ///< lower knuckle point
        KNUCKLE_U,   ///< upper knuckle point
        LL_A,        ///< lower link, axle
        LL_C,        ///< lower link, chassis
        UL_A,        ///< upper link, axle
        UL_C,        ///< upper link, chassis
        SPRING_A,    ///< spring, axle
        SPRING_C,    ///< spring, chassis
        TIEROD_C,    ///< tierod, chassis
        TIEROD_K,    ///< tierod, knuckle
        SPINDLE,     ///< spindle location
        KNUCKLE_CM,  ///< knuckle, center of mass
        LL_CM,       ///< lower link, center of mass
        UL_CM,       ///< upper link, center of mass
        NUM_POINTS
    };

    /// Return the location of the specified hardpoint.
    /// The returned location must be expressed in the suspension reference frame.
    virtual const ChVector<> getLocation(PointId which) = 0;

    /// Return the center of mass of the axle tube.
    virtual const ChVector<> getAxleTubeCOM() const = 0;

    /// Return the mass of the axle tube body.
    virtual double getAxleTubeMass() const = 0;
    /// Return the mass of the spindle body.
    virtual double getSpindleMass() const = 0;
    /// Return the mass of the upper link body.
    virtual double getULMass() const = 0;
    /// Return the mass of the lower link body.
    virtual double getLLMass() const = 0;
    /// Return the mass of the knuckle body.
    virtual double getKnuckleMass() const = 0;

    /// Return the radius of the axle tube body (visualization only).
    virtual double getAxleTubeRadius() const = 0;
    /// Return the radius of the spindle body (visualization only).
    virtual double getSpindleRadius() const = 0;
    /// Return the width of the spindle body (visualization only).
    virtual double getSpindleWidth() const = 0;
    /// Return the radius of the upper link body (visualization only).
    virtual double getULRadius() const = 0;
    /// Return the radius of the lower link body (visualization only).
    virtual double getLLRadius() const = 0;
    /// Return the radius of the knuckle body (visualization only).
    virtual double getKnuckleRadius() const = 0;

    /// Return the moments of inertia of the axle tube body.
    virtual const ChVector<>& getAxleTubeInertia() const = 0;
    /// Return the moments of inertia of the spindle body.
    virtual const ChVector<>& getSpindleInertia() const = 0;
    /// Return the moments of inertia of the upper link body.
    virtual const ChVector<>& getULInertia() const = 0;
    /// Return the moments of inertia of the lower link body.
    virtual const ChVector<>& getLLInertia() const = 0;
    /// Return the moments of inertia of the knuckle body.
    virtual const ChVector<>& getKnuckleInertia() const = 0;

    /// Return the inertia of the axle shaft.
    virtual double getAxleInertia() const = 0;

    /// Return the free (rest) length of the spring element.
    virtual double getSpringRestLength() const = 0;
    /// Return the callback function for spring force.
    virtual ChSpringForceCallback* getSpringForceCallback() const = 0;
    /// Return the callback function for shock force.
    virtual ChSpringForceCallback* getShockForceCallback() const = 0;

    ChSharedPtr<ChBody> m_axleTube;      ///< handles to the axle tube body
    ChSharedPtr<ChBody> m_tierod;        ///< handles to the tierod body
    ChSharedPtr<ChBody> m_knuckle[2];    ///< handles to the knuckle bodies (left/right)
    ChSharedPtr<ChBody> m_upperLink[2];  ///< handles to the upper link bodies (left/right)
    ChSharedPtr<ChBody> m_lowerLink[2];  ///< handles to the lower link bodies (left/right)

    ChSharedPtr<ChLinkLockRevolute>
        m_revoluteKingpin[2];     ///< handles to the knuckle-axle tube revolute joints (left/right)
    ChSharedPtr<ChLinkLockSpherical>
        m_sphericalTierod;        ///< handles to the knuckle-tierod spherical joint (left)
    ChSharedPtr<ChLinkUniversal>
        m_universalTierod;        ///< handles to the knuckle-tierod universal joint (right)
    ChSharedPtr<ChLinkLockSpherical>
        m_sphericalUpperLink[2];  ///< handles to the upper link-axle tube spherical joints (left/right)
    ChSharedPtr<ChLinkLockSpherical>
        m_sphericalLowerLink[2];  ///< handles to the lower link-axle tube spherical joints (left/right)
    ChSharedPtr<ChLinkUniversal>
        m_universalUpperLink[2];  ///< handles to the upper link-chassis universal joints (left/right)
    ChSharedPtr<ChLinkUniversal>
        m_universalLowerLink[2];                  ///< handles to the lower link-chassis universal joints (left/right)
    ChSharedPtr<ChLinkDistance> m_distTierod;  ///< handles to the tierod distance constraint (left)

    ChSharedPtr<ChLinkSpringCB> m_shock[2];   ///< handles to the spring links (left/right)
    ChSharedPtr<ChLinkSpringCB> m_spring[2];  ///< handles to the shock links (left/right)

  private:
    void InitializeSide(VehicleSide side,
                        ChSharedPtr<ChBodyAuxRef> chassis,
                        ChSharedPtr<ChBody> tierod_body,
                        const std::vector<ChVector<> >& points);

    static void AddVisualizationLink(ChSharedPtr<ChBody> body,
                                     const ChVector<> pt_1,
                                     const ChVector<> pt_2,
                                     double radius,
                                     const ChColor& color);
    static void AddVisualizationKnuckle(ChSharedPtr<ChBody> knuckle,
                                        const ChVector<> pt_U,
                                        const ChVector<> pt_L,
                                        const ChVector<> pt_T,
                                        double radius);
    static void AddVisualizationSpindle(ChSharedPtr<ChBody> spindle, double radius, double width);

    static const std::string m_pointNames[NUM_POINTS];
};

}  // end namespace vehicle
}  // end namespace chrono

#endif
