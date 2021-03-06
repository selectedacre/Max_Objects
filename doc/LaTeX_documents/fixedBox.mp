% File: fixedBox.mp



define_global_numeric_option("height");

define_global_numeric_option("width");



%=====================================================================

% Definitions specific to the |FixedBox| class

% A class ``FixedBox'' with four points.



% This is a copy of the |Box| class with the addition of fixed height and with

% and no 'fit' option.



% A constructor initializing the variable |p| (picture)

% |@#| is a name for a fixed box (must be a suffix)

% |@#| will be the number of the fixed box, but will also be used

% as a prefix for other variables.

% |v| is either a picture, a string or an object given by its number

vardef newFixedBox@#(expr v) text options= 

  ExecuteOptions(@#)(options);

  assignObj(@#,"FixedBox");

  StandardInterface;

  StandardObjectOrPictureContainerSetup(v);

  if OptionValue@#("rbox_radius")>0:

    ObjPoint ene,ese,sse,ssw,wsw,wnw,nnw,nne;

    % we use paths for the rounded corners if necessary

    addPathVariables@#(_spath_);

  fi;

  ObjCode StandardEquations,

    if numeric v:

      ".5[@#isw,@#ine]=.5[obj(@#sub)ne,obj(@#sub)sw]", % object

    elseif (picture v) or (string v):

      ".5[@#isw,@#ine]=@#p.off", % picture offset

    fi

    if OptionValue@#("rbox_radius")>0:

      "@#ine-@#nne=@#ise-@#sse=@#nnw-@#inw=@#ssw-@#isw=(" &

        decimal (OptionValue@#("rbox_radius")) & ",0)",

      "@#ine-@#ene=@#ese-@#ise=@#inw-@#wnw=@#wsw-@#isw=(0," &

        decimal (OptionValue@#("rbox_radius")) & ")",

    fi

    "@#ise-@#isw=(" & decimal (OptionValue@#("width")) & ",0)",

    "@#ine-@#ise=(0," & decimal (OptionValue@#("height")) & ")";

  StandardTies;

  if OptionValue@#("rbox_radius")>0:

    addPath@#(_spath_,1,

      @#nnw{left}..{down}@#wnw--@#wsw{down}

      ..{right}@#ssw--@#sse{right}..{up}@#ese--@#ene{up}

      ..{left}@#nne--cycle

      );

    defineFixedBox_pathparameters(@#);

  fi;

enddef;



def defineFixedBox_pathparameters(suffix $)=

  $_spath_.n_:=1;

  $_spath_._draw_[1]:=LocalOptionValue("cdraw","cdraw_default");

  $_spath_.visible[1]:=true;

  $_spath_.pathfilled[1]:=false;

  $_spath_.pathfillcolor[1]:=black;

  $_spath_.border[1]:=CLOV_("border");

  $_spath_.bordercolor[1]:=CLOV_("bordercolor");

  $_spath_.linewidth[1]:=OptionValue$("framewidth");

  $_spath_.linecolor[1]:=OptionValue$("framecolor");

  $_spath_.nodesepA[1]:=0;

  $_spath_.nodesepB[1]:=0;

  $_spath_.arrows[1]:="draw";

  $_spath_.linestyle[1]:=CLOV_("linestyle");

  $_spath_.doubleline[1]:=false;

  forsuffixes $$=_draw_,visible,border,bordercolor,linewidth,linecolor,

    arrows,linestyle,nodesepA,nodesepB,doubleline,pathfilled,pathfillcolor:

    $_spath_$$n_:=1;

  endfor;

enddef;

  

vardef newRFixedBox@#(expr v) text options=

  newFixedBox@#(v) "rbox_radius(1mm)", options;

enddef;



vardef new_RFixedBox(expr v)=

  new_FixedBox_(v)("rbox_radius(1mm)")

enddef;



streamline("FixedBox")("(expr v)","(v)");



def BpathFixedBox(suffix n)=

  (if OptionValue.n("rbox_radius")=0:

     StandardBpath(n)

   else:

     % good curve:

     % |cycle| was added because in certain cases, |unfill| is called

     % on the path returned by |BpathBox|.

     (Path.n(_spath_,1)--cycle)

     % bad curve:     

%     (n.nnw{n.nnw-n.nne}..{n.wsw-n.wnw}n.wnw--n.wsw{n.wsw-n.wnw}

%       ..{n.sse-n.ssw}n.ssw--n.sse{n.sse-n.ssw}

%       ..{n.ene-n.ese}n.ese--n.ene{n.ene-n.ese}

%       ..{n.nnw-n.nne}n.nne--cycle)

   fi

  )

enddef;

    

def drawFixedBox(suffix n)=

  if OptionValue.n("rbox_radius")=0:

    drawFramedOrFilledObject_(n);

  else:

    if OptionValue.n("framed"):

      if OptionValue.n("shadow"):

        fill (BpathObj(n) shifted (1mm,-1mm))

             withcolor OptionValue.n("shadowcolor");

      fi;

      unfill BpathObj(n);

    fi;

    if OptionValue.n("filled"):

      fill BpathObj(n) withcolor OptionValue.n("fillcolor");

    fi;

  fi;

  drawPictureOrObject(n);

  drawMemorizedPaths_(n);

enddef;



setObjectDefaultOption("FixedBox")("filled")(false);

setObjectDefaultOption("FixedBox")("fillcolor")(black);

setObjectDefaultOption("FixedBox")("framed")(true);

setObjectDefaultOption("FixedBox")("shadow")(false); % no shadow by default

setObjectDefaultOption("FixedBox")("shadowcolor")(black);

setObjectDefaultOption("FixedBox")("framewidth")(.5bp);

setObjectDefaultOption("FixedBox")("framecolor")(black);

setObjectDefaultOption("FixedBox")("framestyle")("");

setObjectDefaultOption("FixedBox")("rbox_radius")(0); % after rboxes.mp

setObjectDefaultOption("FixedBox")("picturecolor")(black);

setObjectDefaultOption("FixedBox")("width")(3bp); % same value as in |boxes.mp|

setObjectDefaultOption("FixedBox")("height")(3bp); % same value as in |boxes.mp|



% end of the definition of FixedBox



% $Log: fixedBox.mp,v $
% Revision 1.3  2006/07/20 04:47:51  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.1  2005/08/02 15:09:16  churchoflambda
% Initial version.
%

