// TRENTO: Reduced Thickness Event-by-event Nuclear Topology
// Copyright 2015 Jonah E. Bernhard, J. Scott Moreland
// MIT License

#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <map>

#ifdef NDEBUG
#define BOOST_DISABLE_ASSERTS
#endif
#include <boost/multi_array.hpp>

#include "fwd_decl.h"
#include "nucleon.h"

namespace trento {

class NucleonProfile;

/// \rst
/// The primary computation class, responsible for constructing nuclear
/// thickness functions and calculating event observables.  Designed to be
/// created once and used many times by repeatedly calling ``compute()``.
/// Stores its observables internally and provides inspector methods.
///
/// Example::
///
///   Event event{var_map};
///   for (int n = 0; n < nevents; ++n) {
///     event.compute(nucleusA, nucleusB, nucleon_profile);
///     do_something(
///       event.npart(),
///       event.multiplicity(),
///       event.eccentricity(),
///       event.reduced_thickness_grid()
///     );
///   }
///
/// \endrst
class Event {
 public:
  /// Instantiate from the configuration.
  explicit Event(const VarMap& var_map);

  /// \rst
  /// Compute thickness functions and event observables for a pair of
  /// ``Nucleus`` objects and a ``NucleonProfile``.  The nuclei must have
  /// already sampled nucleon positions and participants before passing to this
  /// function.
  /// \endrst
  void compute(const Nucleus& nucleusA, const Nucleus& nucleusB,
               NucleonProfile& profile);

  // Alias for a two-dimensional thickness grid.
  using Grid = boost::multi_array<double, 2>;

  /// Number of nucleon participants.
  const int& npart() const
  { return npart_; }

  /// WK: Number of binary collision.
  const int& ncoll() const
  { return ncoll_; }

  /// \rst
  /// Multiplicity---or more specifically, total entropy.  May be interpreted
  /// as `dS/dy` or `dS/d\eta` at midrapidity.
  /// \endrst
  const double& multiplicity() const
  { return multiplicity_; }

  /// \rst
  /// Eccentricity harmonics `\varepsilon_n` for *n* = 2--5.
  /// Returns a map of `(n : \varepsilon_n)` pairs, so e.g.::
  ///
  ///   double e2 = event.eccentricity().at(2);
  ///
  /// \endrst
  const std::map<int, double>& eccentricity() const
  { return eccentricity_; }

  const std::map<int, double>& event_planes() const
  { return psi_; }

  /// The reduced thickness grid as a square two-dimensional array.
  const Grid& reduced_thickness_grid() const
  { return TR_; }

  /// WK: The TAB grid for hard process vertex sampling
  const Grid& TAB_grid() const
  { return TAB_; }

  /// WK: The grid step
  const double& dxy() const
  { return dxy_; }

  /// WK: clear and increase TAB
  void clear_TAB(void);
  void accumulate_TAB(Nucleon& A, Nucleon& B, NucleonProfile& profile);

  /// WK:
  const bool& with_ncoll() const
  { return with_ncoll_; }

 private:
  /// Compute a nuclear thickness function (TA or TB) onto a grid for a given
  /// nucleus and nucleon profile.  This destroys any data previously contained
  /// by the grid.
  void compute_nuclear_thickness(
      const Nucleus& nucleus, NucleonProfile& profile, Grid& TX);

  /// Compute the reduced thickness function (TR) after computing TA and TB.
  /// Template parameter GenMean sets the actual function that returns TR(TA, TB).
  /// It is determined at runtime based on the configuration.
  template <typename GenMean>
  void compute_reduced_thickness(GenMean gen_mean);

  /// An instantation of compute_reduced_thickness<GenMean> with a bound
  /// argument for GenMean.  Created in the ctor.  Implemented this way to
  /// allow the compiler to fully inline the GenMean function and only require a
  /// single "virtual" function call per event.
  std::function<void()> compute_reduced_thickness_;

  /// Compute observables that require a second pass over the reduced thickness grid.
  void compute_observables();

  /// Normalization factor.
  const double norm_;

  /// Grid step size.
  const double dxy_;

  /// Number of grid steps.
  const int nsteps_;

  /// Grid xy maximum (half width).
  const double xymax_;

  /// Nuclear thickness grids TA, TB and reduced thickness grid TR.
  Grid TA_, TB_, TR_, TAB_;

  /// Center of mass coordinates in "units" of grid index (not fm).
  double ixcm_, iycm_;

  /// Number of participants.
  int npart_;

  /// WK: Number of binary collisions.
  int ncoll_;

  /// Multiplicity (total entropy).
  double multiplicity_;

  /// Eccentricity harmonics.
  std::map<int, double> eccentricity_;

  /// WK: Initial density event planes.
  std::map<int, double> psi_;

  /// WK:
  bool with_ncoll_;
};

}  // namespace trento

#endif  // EVENT_H
