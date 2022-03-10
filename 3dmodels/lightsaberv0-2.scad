//Fixings
M5=5.5;
M4=4.5;
M3=3.5;
M2=2.5;
M3INSERT=3.7;
M2INSERT=3.4;


//Blade
BladeDiam=23;//24;
BladeDiamTrue=19.5;
BladeLen=100; //Debug only
BladeThick=3;

CapCollar=2;

//Pico
PicoWid=21;
PicoLen=64;
PicoDep=19;

//PIZ
PIZWid=34;
PIZLen=72;
PIZDep=19;
PCBGrid=2.54;

//Stem
StemLen=50;
StemCollar=0;//5;
StemSegLen=6;
StemSegMaxDiam=50;
StemSegStep=6;

//Grip
GripLen=160; //Max print size is 170
GripDiam=40;//45;
GripOverlap=12;

//Pummel
PummelDiam=GripDiam+10;
PummelIntDiam = GripDiam + 1;
PummelLen=20;
PummelOverlap=5;
PowerWid=11.5;
PowerLen=9;


//PCB
PCBWidth=30.5;
PCBLen=135;
PCBBRy=3.9;
PCBBRx=7.35;
PCBBLx=16.51;//15.51;
PCBBLy=9.4;
PCBTRx=0;
PCBTRy=90.79;
PCBLEDy=12.5;
PCBLEDx=10;
PCBLEDz=5;
PCBSwitchD=5 ;
PCBSwitchY=23.5;
PCBSwitchX=7.6;
PCBSwitchZ=15;//12;
PCBSwitchL=20;
PCBCapX=12;
PCBCapY=114;
PCBCapL=20;
PCBCapZ=PCBCapL/2;
PCBCapD=10.2;
PCBOffz=10;
PCBOffy=-10;



THICK=2;

$fn=100;


/*
Blade();
Pico();
PIZ();
*/
//Blade();
//PCB();
//PIR();

    
//BladeCap();

//Stem();
//GripBack();
//PummelInt();
//Pummel();
//translate([5,10,0]) GripFront();
//GripFront();

//Guard();
GuardShort();

//MountBottom();
//MountTop();



//Test print
/*
difference(){
    GripBack();
    translate([0,0,GripLen*(-0.85)])
        cube([PummelDiam, PummelDiam, GripLen], center=true);
}
*/




module Blade(){
    color("blue")
    translate([0,0,BladeLen/2]){
        difference(){
            cylinder(d=BladeDiam, h=BladeLen, center=true);
            cylinder(d=BladeDiam-(BladeThick*2), h=BladeLen+10, center=true);
        }
        
    }
}

module BladeCap(){
    inner = BladeDiamTrue-(BladeThick);
    translate([0,0,BladeLen]){
        //Half hollow Sphere
        difference(){
            sphere(d=BladeDiamTrue, $fn=100);
            sphere(d=inner);
            translate([0,0,BladeDiamTrue/(-2)]){
                cube([BladeDiamTrue+10, BladeDiamTrue+10, BladeDiamTrue], center=true);
            }
            
            
           for (x=[0:30:360]){
               rotate([0,0,x]){
                   rotate([0,65,0])
                      cylinder(d=M3, h=BladeThick*10, center=true);
               }
           }
  
            
        }
        //Connection collar
        /*
        difference(){
            cylinder(d=inner, h=CapCollar*2, center=true);
            cylinder(d=inner-(THICK*2), h=CapCollar*4, center=true);
        }
        */
    }
}

module Stem(){
    //Inner Tube
    translate([0,0,StemLen/(-2)]){
        difference(){
            cylinder(d=BladeDiam+(THICK*2), h=StemLen, center=true);
            cylinder(d=BladeDiam-(THICK), h=StemLen+10, center=true);
            translate([0,0,(StemLen/2)-(StemCollar/2)]){
                cylinder(d=BladeDiam, h=StemCollar, center=true);
            }
            
            //Coupline
            CoupZ=StemLen/(-2) + (GripOverlap/2);
            translate([0, 0, CoupZ]){
                //Hole
                rotate([90,0,0]){
                    cylinder(d=M2, h=BladeDiam+(THICK*4), center=true);
                }
                //Loose top
                translate([0, (BladeDiam+(THICK*2))/2, THICK*(-1)]){
                    //cube([BladeDiam+(THICK*2), BladeDiam+(THICK*2), GripOverlap-THICK], center=true);
                    cube([BladeDiam/2, BladeDiam+(THICK*2), GripOverlap-THICK], center=true);
                }
            }
        }
    }
    //Steps
    difference(){
        translate([0,0,StemSegLen/2]){
            for(i=[0:4]){
                translate([0, 0, (StemSegLen*(i+1))/(-1)]){
                    cylinder(d=StemSegMaxDiam-(StemSegStep*i), h=StemSegLen, center=true);
                }
            }
        }
        //Hole
        translate([0,0,StemLen/(-2)]){
            cylinder(d=BladeDiam, h=StemLen+10, center=true);
        }   
    }
}

function picoZ()=(StemLen + ((PicoLen+(THICK*4)) / (2)))* (-1) ;

module Pico(){
    z=picoZ();
    translate([0,0,z]){
        color("lightgrey"){
            cube([PicoWid, PicoDep, PicoLen], center=true);
        }
    }
}

module PIZ(){
    z=picoZ() - PIZLen ;
    translate([0,0,z]){
        color("lightGreen"){
            cube([PIZWid, PIZDep, PIZLen], center=true);
        }
    }
}


module GripBack(){
    difference(){
        GripTube();
        rotate([0,0,-30]){
            translate([0,GripDiam/2, gripTubeZ()-THICK]){
                cube([GripDiam+10,GripDiam,GripLen ], center=true);
            }
        }
    }
    stemCoupling();
}

module GripFront(){
    intersection(){
        difference(){
            GripTube();
            translate([0,0,gripTubeZ()]){
                cylinder(d=GripDiam - (THICK*2), h=GripLen+10, center=true);
            }
            rotate([0, 0, -30]){
                translate([0,GripDiam/(-2), gripTubeZ()]){
                    cube([GripDiam+10,GripDiam,GripLen+10 ], center=true);
                }
            }
            
            /*
            //Switch hole
            translate([0,PCBOffy,gripTubeZ()]){
                for(i=[-2:1:2]){
                    translate([PCBWidth/-2, PCBSwitchZ + i, PCBLen/-2-5.5+PCBSwitchY]){
                        rotate([0,90,0]){
                            cylinder(d=PCBSwitchD+1,h=20,center=true);
                        }
                    }
                }
            }
            */
        }
        
        rotate([0, 0, -30]){
            translate([0,GripDiam/2, gripTubeZ()-THICK]){
                cube([GripDiam+10,GripDiam,GripLen ], center=true);
            }
        }
    }
}
    
function gripTubeZ() = (StemLen + GripLen/2 - GripOverlap) *( -1);
module GripTube(){
    z=gripTubeZ();
    translate([0,0,z]){
        difference(){
            cylinder(d=GripDiam, h=GripLen, center=true);
            translate([0,0,THICK*(-1)]){
                cylinder(d=GripDiam - (THICK*2), h=GripLen, center=true);
            }
            //Stem Hole
            cylinder(d=BladeDiam+(THICK*2.5), h=GripLen+10, center=true);
            
            //LED hole
            translate([PCBWidth/-2, PCBLEDz*(-1), PCBLen/-2-9.5+PCBLEDy]){
                rotate([0,90,0]){
                    cylinder(d=3,h=20,center=true);
                }
            }
            
            //Switch hole
            translate([PCBWidth/-2, PCBSwitchZ+PCBOffy, PCBLen/-2-7+PCBSwitchY]){
                for(i=[-2:1:2]){
                    translate([0, i, 0]){
                        rotate([0,90,0]){
                            cylinder(d=PCBSwitchD+1.5,h=20,center=true);
                        }
                    }
                }
            }
            
            //PowerHole
            translate([PCBWidth/-2, -4.5, PCBLen/2-5]){
                translate([-5, 0, PowerSocketY*(-1)]){
                    rotate([90,0,0])
                        PowerSocketHole();
                }
            }
        }
    }
    
    stemCoupling();
    
    
    //PCB Mount
    translate([0,0,z-(GripOverlap/2)]){
        difference(){
            //PCB plate
            cylinder(d=GripDiam, h=GripLen-(GripOverlap), center=true);
            cube([GripDiam, PIZDep, GripLen], center=true);
            translate([0,GripDiam/2,0]){
                cube([GripDiam+10, GripDiam, GripLen+10], center=true);
            }
            translate([0,0,PCBLen/(-2)-5]){
                cube([GripDiam, GripDiam, 10], center=true);
            }
            
            //Mounting Holes
            translate([PCBWidth/-2,0, PCBLen/-2-1.5]){
                translate([PCBBRx,0,PCBBRy]){
                    rotate([90,0,0]){
                        cylinder(d=M2INSERT, h=BladeDiam+(THICK*4), center=true);
                    }
                    translate([PCBBLx,0, PCBBLy]){
                        rotate([90,0,0]){
                            cylinder(d=M2INSERT, h=BladeDiam+(THICK*4), center=true);
                        }
                    }
                    translate([PCBTRx,0, PCBTRy]){
                       rotate([90,0,0]){
                            cylinder(d=M2INSERT, h=BladeDiam+(THICK*4), center=true);
                        }
                    }
                }
            }
            
            
            /*
            //PCB Weight Holes
            pcbCountX=GripDiam/PCBGrid/2-2;
            pcbCountY=(GripLen-GripOverlap)/PCBGrid/2;
            for (y=[0:5:pcbCountY]){
                for (j=[-1,1]){
                    translate([0, 0, y*j*PCBGrid]){
                        for (x=[0:3:pcbCountX]){
                            for (i=[-1,1]){
                                translate([x*i*PCBGrid,0,0]){
                                    rotate([90,0,0]){
                                        cylinder(d=M2INSERT, h=BladeDiam+(THICK*4), center=true);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            */
            
        }
    }
}


module stemCoupling(){
    //Coupling for Stem
    translate([0,0,gripTubeZ() + GripLen/2 - GripOverlap/2]){
        difference(){
            cylinder(d=BladeDiam+(THICK*6.5), h=GripOverlap, center=true);
            cylinder(d=BladeDiam+(THICK*2.5), h=GripOverlap+10, center=true);
            //Bottom half only
            //translate([0,BladeDiam/2,0]){
            //    cube([BladeDiam+(THICK*10),BladeDiam+(THICK*4),GripOverlap+10], center=true);
            //}
            
            //Fixing
            rotate([90,0,0]){
                cylinder(d=M2INSERT, h=BladeDiam+(THICK*4), center=true);
            }
            
            //Opening
            translate([0,BladeDiam,0]){
                cube([BladeDiam/2,BladeDiam,BladeDiam], center=true);
            }
        }
    }
}


module Pummel(){
    z=gripTubeZ()-GripLen/2 - PummelLen/2 + PummelOverlap;
    translate([0,0,z]){
        difference(){
            cylinder(d=PummelDiam, h=PummelLen, center=true);
            translate([0,0,THICK]){
                difference(){
                    cylinder(d=PummelIntDiam, h=PummelLen, center=true);
                    translate([0,0,THICK * (-1.5)])
                        cylinder(d=THICK*3, h=PummelLen - THICK *3, center=true);
                }

            }
            cylinder(d=M2, h=PummelLen*2, center=true);
            
            //PIR
            translate([PIRX,PummelDiam/2-3,0]){
                sphere(d=PIRD+0.3);
                translate([0,THICK*(-3),0])
                    rotate([90,0,0]){
                        cylinder(d=PIRD+0.3, h=THICK*6, center=true);
                    }
            }
        }
    }
}







module PCB(){
    translate([0,PCBOffy, (StemLen+ PCBLen/2 + PCBOffz)*(-1)+2.2])
    {
        rotate([-90,180,0]){  
            PCBx();
        }
    }
}

module PCBx(){
    difference(){
        color([0.2,1,0.5])
            cube([PCBWidth, PCBLen, THICK], center=true);            
        PCBholes();
    }
    translate([PCBWidth/2, PCBLen/-2, 0]){
        //LED
        translate([PCBLEDx*(-1), PCBLEDy, PCBLEDz]){
            rotate([0,90,0]){
                color([0,0,1])
                    cylinder(d=PCBLEDz, h=PCBLEDz, center=true);
            }
        }
        //Switch
        translate([PCBSwitchX*(-1)+(PCBSwitchL/2), PCBSwitchY, PCBSwitchZ]){
            rotate([0,90,0]){
                color([0.1,0.1,0.1])
                    cylinder(d=PCBSwitchD, h=PCBSwitchL, center=true);
            }
        }
        //CAP
        translate([PCBCapX*(-1), PCBCapY, PCBCapZ]){
            color([0,0,0])
                cylinder(d=PCBCapD, h=PCBCapL, center=true);
        }
        
        
    }
    
    PowerSocket();
}

module PCBholes(){
    translate([PCBWidth/2, PCBLen/-2, 0]){
        translate([PCBBRx*(-1),PCBBRy,0]){
            cylinder(d=M2, h=THICK*2, center=true);
            translate([PCBBLx*(-1),PCBBLy,0]){
                cylinder(d=M2, h=THICK*2, center=true);
            }
            translate([PCBTRx*(-1),PCBTRy,0]){
                cylinder(d=M2, h=THICK*2, center=true);
            }
        }
    }
}


PowerSocketD=8.6;
PowerSocketX=12;
PowerSocketY=46;
PowerSocketZ=12;//5.5;
PowerSocketHol=16;//14.9;

module PowerSocket(){
    color([0,0,0]){
        translate([PCBWidth/2, PCBLen/2+2, PowerSocketZ-1]){
            translate([0,PowerSocketY*(-1),0]){
                translate([0,PowerSocketD/4,0]){
                    rotate([0,90,0]){
                        cylinder(d=PowerSocketD+0.3, h=PowerSocketX, center=true);
                    }
                }
                translate([0,PowerSocketD/-4,0]){
                    cube([PowerSocketX, PowerSocketD+0.3, PowerSocketD], center=true);
                }
            }
        }
    }
}

module PowerSocketHole(){
    translate([0,0,PowerSocketZ/(-2)]){    
        translate([0,PowerSocketD/4,0]){
            rotate([0,90,0]){
                cylinder(d=PowerSocketD+0.3, h=PowerSocketX, center=true);
            }
        }
        translate([0,PowerSocketD/-4,0]){
            cube([PowerSocketX, PowerSocketD+0.5, PowerSocketD+0.5], center=true);
            //translate([0,0,-3])
            //    cube([PowerSocketX, PowerSocketD+0.3, PowerSocketD], center=true);
        }
    }
    for (i=[2,-2]){
        translate([0,PowerSocketHol/i, PowerSocketZ/(-2)]){
            rotate([0,90,0]){
                cylinder(d=M2, h=PowerSocketX, center=true);
            }
        }
    }
}



module PummelInt(){
    z=gripTubeZ()+(GripLen+THICK)/-2;
    translate([0,0,z]){
        difference(){
            union(){
                cylinder(d=GripDiam, h=THICK, center=true);
                translate([0,0, THICK])
                    cylinder(d=GripDiam-(THICK*2.3), h=THICK, center=true);
            }
            
            //Insert
            cylinder(d=M2INSERT,h=THICK*4, center=true);
            
            //Gap for Cables
            gapD = GripDiam - THICK * 6;
            for (i=[0,180]){
                rotate([0,0,i]){
                    difference(){
                        cylinder(d=gapD, h=THICK*4, center=true);

                        translate([0, THICK * 5, 0]){
                            cube([gapD, gapD, gapD], center=true);
                        }
                    }
                }
            }
            
        }
    }
}


PIRD=12.9;
PIRW=10.1;
PIRL=30;
PIRX=5;

module PIR(){
    color([0.5,0.5,0.5]){
        z=gripTubeZ()+(GripLen+THICK)/-2-PummelLen/4;
        translate([PIRX,PummelDiam/2-THICK*2,z]){
            difference(){
                sphere(d=PIRD); 
                translate([0,PIRD/-2,0]){
                    cube([PIRD,PIRD,PIRD], center=true);
                }
            }
            translate([0,(PIRL)/-2,0])
                cube([THICK,PIRL,PIRW], center=true);
        }
    }
    
}

GuardLen=THICK*4;
GuardDiam = PummelDiam;
GuardIntDiam = PummelIntDiam;
GuardWid = 15;


module Guard(){
    z=gripTubeZ()+GripLen/2;
    translate([0,0,z]){
        difference(){
            union(){
                cylinder(d=GuardDiam, h=GuardLen, center=true);
                translate([0,THICK*2,GripLen/-2+THICK*4])
                    GuardFly();
            }
            translate([0,0,THICK*(-1)]){
                cylinder(d=GuardIntDiam, h=GuardLen, center=true);
            }   
            cylinder(d=BladeDiam+(THICK*2.5), h=GuardLen+10, center=true);
        }
        
    }
}

module GuardFlyOld(){
    intersection(){
        translate([GripLen*2/-3-THICK*7,0,0])
            difference(){ 
                sphere(d=GripLen*2);
                sphere(d=GripLen*2-THICK*2);
                
            }
        //translate([GripDiam/2,GuardDiam/-4,GripLen/-2])
        //    cube([GuardWid*2,GuardWid,GripLen]);
            
        translate([GripDiam-THICK*4,(GuardWid/-4),THICK*(-3)]){
            intersection(){
                scale([2,1,360])
                    sphere(d=GuardWid);
                cube([GuardWid*2,GuardWid,GripLen], center=true);
            }
        }
    }
    
}

module GuardFly(){
    intersection(){
        translate([0,0,0])
            scale([1,1,(GripLen/PummelDiam)*1.7]){ 
                difference(){
               
                    sphere(d=PummelDiam);
                    sphere(d=PummelDiam-THICK*3);
               }
                
            }

        rotate([0,0,90]){
            translate([THICK*6,(GuardWid/-4),THICK*(-3)]){
                intersection(){
                    scale([2,1,360])
                        sphere(d=GuardWid);
                    cube([GuardWid*2,GuardWid,GripLen], center=true);
                }
            }
        }
        
    }
    
}


module GuardShort(){
    z=gripTubeZ()+GripLen/2;
    translate([0,0,z]){
        difference(){
            union(){
                cylinder(d=GuardDiam, h=GuardLen, center=true);;
            }
            translate([0,0,THICK*(-1)]){
                cylinder(d=GuardIntDiam, h=GuardLen, center=true);
            }   
            cylinder(d=BladeDiam+(THICK*2.5), h=GuardLen+10, center=true);
        }
        
    }
}


MountWid=50;
MountDep=50;
MountLen=40;

module MountBottom(){
    translate([0,0,StemLen/-1+10]){
        difference(){
            translate([0,MountDep/-10,0]){
                cube([MountWid, MountDep, MountLen], center=true);
                translate([0,MountDep/2,MountLen/2-MountDep/20]){
                    rotate([0,90,0])
                        cylinder(d=MountDep/10, h=MountWid, center=true);
                }
            }
            //Hole
            cylinder(d=StemSegMaxDiam-(StemSegStep*3)+1, h=MountLen*2, center=true);
            //Inside
            translate([0,MountDep/-10,THICK*(-1)])
                cube([MountWid-(THICK*2), MountDep-THICK*3, MountLen], center=true);
            //Slot
            translate([0,MountDep/2,0])
                cube([StemSegMaxDiam-(StemSegStep*4)+1 ,MountDep, MountLen*2], center=true);
            //Mount Holes
            rotate([90,0,0]){
                for (i=[-7.5,7.5]){
                    translate([i,i,0])
                        cylinder(d=M4, h=MountDep*2, center=true);
                }
            }
            
            //Sides
            translate([0,MountDep/-10,MountDep/-10]){
                rotate([0,90,0]){
                    translate([MountDep/2, MountWid/2, 0])
                        cylinder(d=MountWid*1.8, h=MountWid*2, center=true);
                }
            }
        }
    }
}


module MountTop(){
    translate([0,0,100]){
        difference(){
            translate([0,MountDep/-10,0]){
                cube([MountWid, MountDep, MountLen], center=true);
                translate([0,MountDep/2,MountLen/-2+MountDep/20]){
                    rotate([0,90,0])
                        cylinder(d=MountDep/10, h=MountWid, center=true);
                }
            }
            //Hole
            cylinder(d=BladeDiam+1, h=MountLen*2, center=true);
            //Inside
            translate([0,MountDep/-10,THICK])
                cube([MountWid-(THICK*2), MountDep-THICK*3, MountLen], center=true);
           
            //Mount Holes
            rotate([90,0,0]){
                for (i=[-7.5,7.5]){
                    translate([i,i,0])
                        cylinder(d=M4, h=MountDep*2, center=true);
                }
            }
            
            //Sides
            translate([0,MountDep/-7, MountWid*1.1]){
                rotate([0,90,0]){
                    translate([MountDep/2, MountWid/2, 0])
                        cylinder(d=MountWid*1.8, h=MountWid*2, center=true);
                }
            }
            
        }
    }
}


