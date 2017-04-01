# Hacking

If you are working out of the repo, you must be capable of running
`autoreconf -vfi`. Please install your Operating System’s
[`autoconf-archive`](https://packages.gentoo.org/package/sys-devel/autoconf-archive)
package and `pkg-config`. If you refuse or are running Windows, you
may directly obtain `m4-ax_cxx_compile_stdcxx_11.m4` from the
autoconf-archive and place it in `m4/`. You may also acquire `pkg.m4`
from a dist tarball of `pkg-config` and place that in `m4/`.

The primary development platform is mingw32. We use the installer for
mingw-get to achieve this.
