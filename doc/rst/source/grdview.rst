.. index:: ! grdview
.. include:: module_core_purpose.rst_

*******
grdview
*******

|grdview_purpose|

Synopsis
--------

.. include:: common_SYN_OPTs.rst_

**gmt grdview** *reliefgrid* |-J|\ *parameters*
[ |SYN_OPT-B| ]
[ |-C|\ [*cpt*]]
[ |-G|\ *drapegrid* \| |-G|\ *grd_r* |-G|\ *grd_g* |-G|\ *grd_b* ]
[ |-I|\ [*intensgrid*\|\ *intensity*\|\ *modifiers*] ]
[ |-Jz|\ \|\ **Z**\ *parameters* ]
[ |-N|\ [*level*]\ [**+g**\ *fill*] ]
[ |-Q|\ *args*\ [**+m**] ]
[ |SYN_OPT-Rz| ]
[ |-S|\ *smooth* ]
[ |-T|\ [**+o**\ [*pen*]][**+s**] ]
[ |SYN_OPT-U| ]
[ |SYN_OPT-V| ]
[ |-W|\ **c|m|f**\ *pen* ]
[ |SYN_OPT-X| ]
[ |SYN_OPT-Y| ]
[ |SYN_OPT-f| ]
[ |SYN_OPT-n| ]
[ |SYN_OPT-p| ]
[ |SYN_OPT-t| ]
[ |SYN_OPT--| ]

.. module_common_begins

Description
-----------

**grdview** reads a 2-D grid file and produces a 3-D perspective plot by
drawing a mesh, painting a colored/gray-shaded surface made up of
polygons, or by scanline conversion of these polygons to a raster image.
Options include draping a data set on top of a surface, plotting of
contours on top of the surface, and apply artificial illumination based
on intensities provided in a separate grid file.

Required Arguments
------------------

*reliefgrid*
    2-D gridded data set to be imaged (the relief of the surface). (See
    :ref:`Grid File Formats <grd_inout_full>`).

.. |Add_-J| replace:: |Add_-J_links|
.. include:: explain_-J.rst_
    :start-after: **Syntax**
    :end-before: **Description**

Optional Arguments
------------------

.. |Add_-B| replace:: |Add_-B_links|
.. include:: explain_-B.rst_
    :start-after: **Syntax**
    :end-before: **Description**

.. _-C:

.. include:: use_cpt_grd.rst_

.. _-G:

|-G|\ *drapegrid* \| |-G|\ *grd_r* |-G|\ *grd_g* |-G|\ *grd_b*
    Drape the image in *drapegrid* on top of the relief provided by
    *reliefgrid*. [Default determines colors from *reliefgrid*]. Note that **-Jz** and
    **-N** always refers to the *reliefgrid*. The *drapegrid* only
    provides the information pertaining to colors, which (if *drapegrid* is a grid) will be looked-up
    via the CPT (see **-C**). Instead, you may give three grid files
    via separate **-G** options in the specified order. These files must contain the red, green, and
    blue colors directly (in 0-255 range) and no CPT is needed. The
    *drapegrid* may be of a different resolution than the *reliefgrid*.
    Finally, *drapegrid* may be an image to be draped over the surface, in which
    case the **-C** option is not required.

.. _-I:

**-I**\ [*intensgrid*\|\ *intensity*\|\ *modifiers*]
    Gives the name of a grid file with intensities in the (-1,+1) range,
    or a constant intensity to apply everywhere (affects the ambient light).
    Alternatively, derive an intensity grid from the input data grid *reliefgrid*
    via a call to :doc:`grdgradient`; append **+a**\ *azimuth*, **+n**\ *args*,
    and **+m**\ *ambient* to specify azimuth, intensity, and ambient arguments
    for that module, or just give **+d** to select the
    default arguments (**+a**\ -45\ **+nt**\ 1\ **+m**\ 0). If you want a more
    specific intensity scenario then run :doc:`grdgradient` separately first.
    If we should derive intensities from another file than *reliefgrid*, specify the file
    [Default is no illumination].

.. _-Jz:

.. include:: explain_-Jz.rst_

.. _-N:

**-N**\ [*level*]\ [**+g**\ *fill*]
    Draws a plane at this z-level. If the optional *color* is provided
    via the **+g** modifier, and the projection is not oblique,
    the frontal facade between the plane and the data perimeter is
    colored. See **-Wf** for setting the pen used for the outline.
    If no *level* is set then we default to the minimum value in the
    *reliefgrid*. However, if **-R** was used to set *zmin/zmax* then we
    use that value if it is less than the grid minimum value.

.. _-Q:

**-Q**\ *args*\ [**+m**]
    Select one of following settings. For any of these choices, you may force
    a monochrome image by appending the modifier **+m**. Colors are then
    converted to shades of gray using the (monochrome television) YIQ transformation

    #. Specify **m** for mesh plot [Default], and optionally append *color* for a different mesh paint [white].
    #. Specify **mx** or **my** for waterfall plots (row or column profiles). Specify color as for plain **m**
    #. Specify **s** for surface plot, and optionally append **m** to have mesh lines drawn on top of surface.
    #. Specify **i** for image plot, and optionally append the effective dots-per-unit resolution for the rasterization [Default is :term:`GMT_GRAPHICS_DPU`].
    #. Specify **c**. Same as **-Qi** but will make nodes with z = NaN transparent, using the colormasking
       feature in PostScript Level 3 (the PS device must support PS Level 3).

    **Note**: If the CPT is categorical then only **-Qm** is available (but see **-T**).

.. |Add_-R| replace:: |Add_-R_links|
.. include:: explain_-R.rst_
    :start-after: **Syntax**
    :end-before: **Description**

.. |Add_-Rz| replace:: This option may be used to
    indicate the range used for the 3-D axes [Default is region given by
    the *reliefgrid*]. You may ask for a larger *w/e/s/n* region to
    have more room between the image and the axes. A smaller region than
    specified in the *reliefgrid* will result in a subset of the grid.
.. include:: explain_-Rz.rst_

.. _-S:

**-S**\ *smooth*
    Smooth the contours before plotting (see :doc:`grdcontour`) [Default is no smoothing].

.. _-t:

**-T**\ [**+o**\ [*pen*]][**+s**]
    Plot image without any interpolation. This involves converting each
    node-centered bin into a polygon which is then painted separately.
    Append **+s** to skip nodes with z = NaN. This option is suitable for
    categorical data where interpolating between values is meaningless
    and a categorical CPT has been provided via **-C**.
    Optionally, append **+o** to draw the tile outlines, and specify a
    custom pen if the default pen is not to your liking. As this option
    produces a flat surface it cannot be combined with **-JZ** or **-Jz**.

.. |Add_-U| replace:: |Add_-U_links|
.. include:: explain_-U.rst_
    :start-after: **Syntax**
    :end-before: **Description**

.. |Add_-V| replace:: |Add_-V_links|
.. include:: explain_-V.rst_
    :start-after: **Syntax**
    :end-before: **Description**

.. _-W:

**-W**\ **c**\|\ **m**\|\ **f**\ *pen*

    **-Wc**
        Draw contour lines on top of surface or mesh (not image). Append pen
        attributes used for the contours. [Default: width = 0.75p, color =
        black, style = solid].
    **-Wm**
        Sets the pen attributes used for the mesh. [Default: width = 0.25p,
        color = black, style = solid]. You must also select **-Qm** or
        **-Qsm** for meshlines to be drawn.
    **-Wf**
        Sets the pen attributes used for the facade. [Default: width =
        0.25p, color = black, style = solid]. You must also select **-N**
        for the facade outline to be drawn.

.. |Add_-XY| replace:: |Add_-XY_links|
.. include:: explain_-XY.rst_
    :start-after: **Syntax**
    :end-before: **Description**

.. |Add_-f| unicode:: 0x20 .. just an invisible code
.. include:: explain_-f.rst_

.. include:: explain_-n.rst_

.. |Add_perspective| unicode:: 0x20 .. just an invisible code
.. include:: explain_perspective.rst_

.. include:: explain_-t.rst_

.. include:: explain_help.rst_

.. include:: explain_grdresample.rst_

.. module_common_ends

Examples
--------

.. include:: explain_example.rst_

.. include:: oneliner_info.rst_

To make a mesh plot from the file hawaii_grav.nc and drawing the
contours given in the CPT hawaii.cpt on a Lambert map at
1.5 cm/degree along the standard parallels 18 and 24, with vertical
scale 20 mgal/cm, and looking at the surface from SW at 30 degree
elevation, run::

    gmt grdview hawaii_grav.nc -Jl18/24/1.5c -Chawaii.cpt -Jz0.05c -Qm -N-100 -p225/30 -Wc -pdf hawaii_grav_image

To create an illuminated color perspective plot of the gridded data set
image.nc, using the CPT color.cpt, with linear scaling at
10 cm/x-unit and tickmarks every 5 units, with intensities provided by
the file intens.nc, and looking from the SE, use::

    gmt grdview image.nc -Jx10c -Ccolor.cpt -Qs -p135/30 -Iintens.nc -pdf image3D

To make the same plot using the rastering option with dots-per-cm of 50, use::

    gmt grdview image.nc -Jx10c -Ccolor.cpt -Qi50c -p135/30 -Iintens.nc -pdf image3D

To create a color perspective plot of the gridded data set
magnetics.nc, using the CPT mag_intens.cpt, draped over
the relief given by the file topography.nc, with Mercator map width of 6
inch and tickmarks every 1 degree, with intensities provided by the file
topo_intens.nc, and looking from the SE, run::

    gmt grdview topography.nc -JM6i -Gmagnetics.nc -Cmag_intens.cpt -Qs -p140/30 -Itopo_intens.nc -pdf draped3D

.. module_note_begins

Notes
-----

For the **-Qs** option: The PostScript language has no mechanism for smoothly varying
colors within a polygon, so colors can only vary from polygon to
polygon. To obtain smooth images this way you may resample the grid
file(s) using :doc:`grdsample` or use a finer grid size when running
gridding programs like :doc:`surface` or :doc:`nearneighbor`. Unfortunately,
this produces huge PostScript files. The alternative is to use the
**-Qi** option, which computes bilinear or bicubic continuous color
variations within polygons by using scanline conversion to image the polygons.

.. include:: macos_preview_issue.rst_

.. module_note_ends

See Also
--------

:doc:`gmt`,
:doc:`gmtcolors`,
:doc:`grdcontour`,
:doc:`grdimage`,
:doc:`grdsample`,
:doc:`nearneighbor`,
:doc:`basemap`,
:doc:`contour`, :doc:`text`,
:doc:`surface`
