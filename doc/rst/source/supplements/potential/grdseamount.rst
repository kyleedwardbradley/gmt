.. index:: ! grdseamount
.. include:: ../module_supplements_purpose.rst_

***********
grdseamount
***********

|grdseamount_purpose|

Synopsis
--------

.. include:: ../../common_SYN_OPTs.rst_

**gmt grdseamount** [ *table* ]
|-G|\ *outgrid*
|SYN_OPT-I|
|SYN_OPT-R|
[ |-A|\ [*out*/*in*] ]
[ |-C|\ [**c**\|\ **d**\|\ **g**\|\ **o**\|\ **p**] ]
[ |-D|\ *unit* ]
[ |-E| ]
[ |-F|\ [*flattening*] ]
[ |-H|\ *H*/*rho_l*/*rho_h*\ [**+d**\ *densify*][**+p**\ *power*] ]
[ |-K|\ [*densitymodel*] ]
[ |-L|\ [*cut*] ]
[ |-M|\ [*list*] ]
[ |-N|\ *norm* ]
[ |-Q|\ *bmode*/*fmode*\ [**+d**] ]
[ |-S|\ *scale* ]
[ |-T|\ *t0*\ [/*t1*/*dt*]\ [**+l**] ]
[ |-Z|\ *level* ]
[ |SYN_OPT-V| ]
[ |-W|\ *avedensity* ]
[ |SYN_OPT-bi| ]
[ |SYN_OPT-e| ]
[ |SYN_OPT-f| ]
[ |SYN_OPT-i| ]
[ |SYN_OPT-r| ]
[ |SYN_OPT--| ]

|No-spaces|

Description
-----------

**grdseamount** will compute the combined topographic shape of multiple synthetic seamounts given their individual shape
parameters.  We read from *table* (or standard input) a list of seamount locations and sizes and can evaluate either
Gaussian, parabolic, conical, polynomial or disc shapes, which may be circular or elliptical, and optionally
truncated. Various scaling options are available to modify the result, including an option to add in
a background depth (more complicated backgrounds may be added separately via :doc:`grdmath </grdmath>`).
The input data must contain *lon*, *lat*, *radius*, *height* for each seamount.
For elliptical features (requires **-E**) we expect *lon*, *lat*, *azimuth*, *semi-major*, *semi-minor*,
*height* instead. If seamount flattening is specified (via **-F**) with no value appended
then a final column with *flattening* is expected (cannot be used for plateaus).
For temporal evolution of topography the **-T** option may be used, in which case the
data file must have two final columns with the start and stop time of seamount construction.
In this case you may choose to write out a cumulative shape or just the increments produced
by each time step (see **-Q**).  Finally, for mixing different seamount shapes in the input *table*
you can use the trailing text to give the shape code by using **-C** without an argument.

Required Arguments (if **-L** not given)
----------------------------------------

.. |Add_intables| unicode:: 0x20 .. just an invisible code
.. include:: ../../explain_intables.rst_

.. _-G:

.. |Add_outgrid| replace:: Give the name of the output grid file. If |-T| is set then *outgrid* must be a filename
    template that contains a floating point format (C syntax).  If the filename template also contains
    either %s (for unit name) or %c (for unit letter) then we use the corresponding time (in units specified in |-T|)
    to generate the individual file names, otherwise we use time in years with no unit.
.. include:: /explain_grd_inout.rst_
    :start-after: outgrid-syntax-begins
    :end-before: outgrid-syntax-ends

.. _-I:

.. include:: ../../explain_-I.rst_

.. |Add_-R| replace:: |Add_-R_links|
.. include:: ../../explain_-R.rst_
    :start-after: **Syntax**
    :end-before: **Description**

Optional Arguments
------------------

.. _-A:

**-A**\ [*out/in*]
    Build a mask grid; append outside/inside values [1/NaN].
    Here, height and flattening are ignored and **-L**, **-N** and **-Z** are disallowed.

.. _-C:

**-C**\ [**c**\|\ **d**\|\ **g**\|\ **o**\|\ **p**]
    Select seamount shape function: choose among **c** (cone), **d** (disc), **g** (Gaussian)
    **o** (polynomial) and **p** (parabolic) shapes [Default is Gaussian].  All but the disc can
    furthermore be truncated via a flattening parameter *f* set by **-F**.  If **-C** is not given any
    argument then we will read the shape code from the trailing text.  If **-C** is not given
    at all then we default to Gaussian shapes [**g**].  **Note**: The polynomial model has an amplitude
    for a normalized radius *r* that is given by :math:`h(r) = \frac{(1+r)^3(1-r)^3)}{1+r^3}`.  It is
    very similar to the Gaussian model (volume is just ~2.4% larger) but *h* goes exactly to zero at
    the basal radius.

.. figure:: /_images/GMT_seamount_types.*
   :width: 500 px
   :align: center

   The five types of seamounts selected via option **-C**.  In all cases, :math:`h_0` is the maximum
   *height*, :math:`r_0` is the basal *radius*, :math:`h_c` is the noise floor set via **-L** [0], and
   *f* is the *flattening* set via **-F** [0]. The top radius :math:`r_t` is only nonzero if there is
   flattening and hence does not apply to the disc model.

.. _-D:

**-D**\ *unit*
    Append the unit used for horizontal distances in the input file (see `Units`_).
    Does not apply for geographic data (|SYN_OPT-f|) which we convert to km.

.. _-E:

**-E**
    Elliptical data format. We expect the input records to contain
    *lon, lat, azimuth, semi-major, semi-minor, height* (with  the latter in meter)
    for each seamount.  [Default is Circular data format, expecting
    *lon, lat, radius, height*].

.. figure:: /_images/GMT_seamount_map.*
   :width: 500 px
   :align: center

   Use **-E** to select elliptical rather than circular shape in map view.  Both shapes require
   lon, lat. Circular only requires the radius :math:`r_0` while elliptical requires the azimuth
   :math:`\alpha` and the major and minor semi-axes .

.. _-F:

**-F**\ [*flattening*]
    Seamounts are to be truncated to guyots.  Append *flattening* from 0 (no flattening) to 1
    (**Note**: no feature will be produced!), otherwise we expect to find the flattening in
    the last input column [no truncation].  Ignored if used with **-Cd**.

.. _-H:

**-H**\ *H*/*rho_l*/*rho_h*\ [**+d**\ *densify*][**+p**\ *power*]
    Set reference seamount parameters for an *ad-hoc* variable radial density function with depth. Give
    the low and high seamount densities in kg/m^3 or g/cm^3 and the fixed reference height *H* in meters.
    Use modifiers **+d** and **+p** to change the water-pressure-driven flank density increase
    over the full reference height [0] and the variable density profile exponent *power* [1, i.e., a linear change].
    Below, *h(r)* is the final height of any seamount and *z(r)* is a point inside the seamount.  If the seamount is
    truncated (via **-F**) then *h(r)* refers to the untruncated height.  **Note**: If **-V** is used
    the we report the mean density for each seamount processed.  The radial density function is thus defined
    by :math:`\Delta \rho_s = \rho_h - \rho_l` and the *densify* setting :math:`\Delta \rho_f`:

.. math::

    \rho(r,z) = \rho_l + \Delta \rho_f \left (\frac{H-h(r)}{H} \right ) + \Delta \rho_s \left ( \frac{h(r)-z(r)}{H} \right )^p

.. figure:: /_images/GMT_seamount_density.*
   :width: 500 px
   :align: center

   A linear density distribution selected via option **-H**.  Flank density can be affected by water
   pressure if :math:`\Delta \rho_f > 0` while the normalized internal density gradient is raised to
   power *p* to allow for nonlinear gradients.  **Note**: The reference height *H* refers to a very tall
   seamount for which the supplied densities are suitable.  Smaller seamounts will thus see lower core
   densities by virtue of being smaller.

.. _-K:

**-K**\ *densitymodel*
    Append a file name to hold a crossection grid with the predicted densities of the reference model.
    We use normalized coordinates (*x* goes from -1 to +1) and *z* from 0 to 1, both in increments
    of 0.005, yielding a 401 x 201 grid. **Note**: This option can be used without creating the
    seamount grid, hence **-R**, **-I**, **-G**, and **-D** are not required.

.. _-L:

**-L**\ [*cut*]
    List area, volume, and mean height for each seamount; No grid is created.
    Optionally, append the noise-floor cutoff level below which we ignore area and volume [0].

.. _-M:

**-M**\ [*list*]
    Write the times and names of all grids that were created to the text file *list*.
    Requires **-T**.  If not *list* file is given then we write to standard output.
    The output listing is suitable to be used as input to :doc:`grdflexure </supplements/potential/grdflexure>`.
    **Note**: If **-W** is used the we write the relief grid name first followed by the density grid name.  Thus,
    the output records contain *time reliefgrid* [ *densitygrid* ] *timetag*.

.. _-N:

**-N**\ *norm*
    Normalize grid so maximum grid height equals *norm* [no normalization].

.. _-Q:

**-Q**\ *bmode*/*fmode*\ [**+d**]
    Can only be used in conjunction with **-T**.  Append two different modes settings separated by a slash:
    The *bmode* determines how we construct the surface: Specify **c** for cumulative
    volume through time [Default], or **i** for incremental volume added for each time slice.
    The *fmode* determines the volume flux curve we use: Give **c** for a constant volume flux or
    **g** for a Gaussian volume flux [Default] between the start and stop times of each feature. These fluxes
    integrate to a linear or error-function volume fraction over time, respectively, as shown below.
    By default we compute the exact cumulative and incremental values for the seamounts specified.  Append
    **+d** to instead approximate each incremental layer by a disc of constant thickness.

.. figure:: /_images/GMT_seamount_cum_inc.*
   :width: 500 px
   :align: center

   Use *bmode* in **-Q** to choose between cumulative output (**c**; actual topography as function
   of time [left]) or incremental output (**i**; the difference in actual topography over five
   time-steps [right]).  Here we used **-Cg** for a Gaussian model with no flattening and a linear volume flux.

.. figure:: /_images/GMT_seamount_flux.*
   :width: 500 px
   :align: center

   Use *fmode* in **-Q** to choose between a constant (**c**; dashed line) or Gaussian (**g**; heavy line)
   volume flux model.

.. _-S:

**-S**\ *scale*
    Sets optional scale factor for radii [1].

.. _-T:

**-T**\ *t0*\ [/*t1*/*dt*]\ [**+l**]
    Specify *t0*, *t1*, and time increment (*dt*) for sequence of calculations
    [Default is one step, with no time dependency].  For a single specific time, just
    give start time *t0*. Default *unit* is years; append **k** for kyr and **M** for Myr.
    For a logarithmic time scale, append **+l** and specify *n* steps instead of *dt*.
    Alternatively, give a file with the desired times in the first column (these times
    may have individual units appended, otherwise we assume year).  Note that a grid
    will be written for all time-steps even if there are no loads or no changes.

.. |Add_-V| replace:: |Add_-V_links|
.. include:: /explain_-V.rst_
    :start-after: **Syntax**
    :end-before: **Description**

.. _-W:

**-W**\ *avedensity*
    Give the name of the vertically averaged density grid file. If |-T| is set then *avedensity* must be a filename
    template that contains a floating point format (C syntax).  If the filename template also contains
    either %s (for unit name) or %c (for unit letter) then we use the corresponding time (in units specified in |-T|)
    to generate the individual file names, otherwise we use time in years with no unit.

.. _-Z:

**-Z**\ *level*
    Set the background water depth [0]. As all seamounts have positive relief, you may
    use a larger negative *level* to place them under water.

.. |Add_-bi| replace:: [Default is 4 input columns].
.. include:: ../../explain_-bi.rst_

.. |Add_-e| unicode:: 0x20 .. just an invisible code
.. include:: ../../explain_-e.rst_

|SYN_OPT-f|
    Geographic grids (dimensions of longitude, latitude) will be converted to
    km via a "Flat Earth" approximation using the current ellipsoid parameters.

.. |Add_-h| replace:: Not used with binary data.
.. include:: ../../explain_-h.rst_

.. include:: ../../explain_-icols.rst_

.. |Add_nodereg| unicode:: 0x20 .. just an invisible code
.. include:: ../../explain_nodereg.rst_

.. include:: ../../explain_colon.rst_

.. include:: ../../explain_help.rst_

.. include:: ../../explain_distunits.rst_


Examples
--------

To compute the incremental loads from two elliptical, truncated Gaussian seamounts being constructed
from 3 Ma to 2 Ma and 2.8 M to 1.9 Ma using a constant volumetric production rate,
and output an incremental grid every 0.1 Myr from 3 Ma to 1.9 Ma, we can try:

::

    cat << EOF > t.txt
    #lon lat azimuth, semi-major, semi-minor, height tstart tend
    0	0	-20	120	60	5000	3.0M	2M
    50	80	-40	110	50	4000	2.8M	21.9M
    EOF
    gmt grdseamount -R-1024/1022/-1122/924+uk -I2000 -Gsmt_%3.1f_%s.nc t.txt -T3M/1.9M/0.1M -Qi/c -Dk -E -F0.2 -Cg -Ml.lis

The file l.lis will contain records with numerical time, gridfile, and unit time.

See Also
--------

:doc:`gmt.conf </gmt.conf>`,
:doc:`gmt </gmt>`,
:doc:`grdmath </grdmath>`,
:doc:`gravfft </supplements/potential/gravfft>`,
:doc:`grdflexure </supplements/potential/grdflexure>`
