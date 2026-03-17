#ifndef StAnaCuts_H
#define StAnaCuts_H

/* **************************************************
 *
 *  Authors: Guannan Xie <guannanxie@lbl.gov>
 *           Mustafa Mustafa <mmustafa@lbl.gov>
 *
 * **************************************************
 */

#include "Rtypes.h"
#include <string>
#include <array>

namespace anaCuts
{
   std::array<unsigned int, 4> const triggers = {
   860001, 860002, 860011, 860012
   };//200GeV RFF+FF OO 2021 MB

   /***************************************************************/
   // QA event cuts
   float const qavz = 30;// < cm.
   float const qaVerror = 1.0e-5; //
   float const qaVr = 2.0; //cm
   float const qavzVpdVz = 3; //cm

   // QA tracks cuts
   int const qaNHitsFit = 40;
   float const qanHitsFitRatio = 0.52;
   int const qaNHitsDedx = 30;
   float const qaDca = 3;// < cm
   float const qaGPt = 0.2;
   float const qaEta = 1; 
   /***************************************************************/

   /***************************************************************/
   // event cuts 
   float const vz = 30;// < cm.
   float const Verror = 1.0e-5; //
   float const Vr = 2.0; //cm
   float const vzVpdVz = 3; //cm

   // tracks cuts
   int const NHitsFit = 40;
   float const NHitsFitRatio = 0.52;
   int const NHitsDedx = 30;
   float const Dca = 1;// < cm
   float const GPt = 0.2;
   float const Eta = 1;
   /***************************************************************/

   float const ptMin_Re = 0.2;
   float const ptMax_Re = 2.0;
}
#endif
