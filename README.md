# tauargus — hitas

The **hitas** module implements **HiTaS** — the hierarchical table
suppression step of τ-ARGUS. It protects tables by combining the CSP
linear-programming backend (`../csp`, HiGHS-backed) with the hierarchical
structure of a response table.

It calls into the CSP engine and therefore links against `tauargus::csp`.
Every legacy solver name is dispatched to the single HiGHS-backed CSP API;
the historical CPLEX / XPRESS code paths and their solver headers have been
removed.

## What it provides

- **HiTaS solver** — `HiTaSCtrl` (the public control object) and `WrapCSP`
  (the thin wrapper over the CSP engine). Used for the hierarchical /
  optimal cell-suppression pass.
- The module is built with `-DSECBOUNDS` (secondary bounds enabled), matching
  the original build.

The legacy SWIG JNI wrapper and the CPLEX/XPRESS headers are gone; bindings
to the new stack are generated with pybind11
(`python/cpp/bind_hitas.cpp` in the top-level repo).

## Build

Part of the superbuild in the top-level repo (it links `tauargus::csp`):

```bash
cmake -S native -B native/build \
      -DHighs_DIR=$(brew --prefix)/lib/cmake/Highs
cmake --build native/build
```

Produces `libtauargus_hitas.dylib` (shared).

## License

τ-ARGUS is © Statistics Netherlands and is distributed under the EUPL v1.1 /
EUPL-1.2. See the top-level repository for the full notice.
