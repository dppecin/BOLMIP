#ifndef CONSTANTS_H
#define CONSTANTS_H

/* 
-- For Historical Instances, always use EPS=1.0e-5; for Rand and Bent instances, always use EPS=1.0e-4 

-- For Historical Instances, always use EPSCPL=1.0e-7; for Rand and Bent instances, always use EPSCPL=1.0e-6

-- For Historical Instances, always use LINEGENDELTA=1000000; for Rand and Bent instances, always use LINEGENDELTA=100000

-- For any BLM algorithm (for instance, Recursive, PureLex and SPureLex, SPureLex-PWL), activate #define USEPQUEUE and #define BLMALGO; For eTM, deactivate #define USEPQUEUE and #define BLMALGO

-- For special treatment of rectangles with corner points with same IP parts, activate #define IMPROVEMENTNOGOOD

*/

/* Activate the following #defines to use the Recursive Algorithm for Historical Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
#define EPS 1.0e-5
#define EPSCPL 1.0e-7
#define LINEGENDELTA 1000000

#define USEPQUEUE 
#define IMPROVEMENTNOGOOD   
#define BLMALGO      
#define RECURSIVEALGO  
/* ---------------------------------------------------------------------------------------------------------------------------------------- */

/* Activate the following #defines to use the Recursive Algorithm for Rand and Bent Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-4
//#define EPSCPL 1.0e-6
//#define LINEGENDELTA 100000

//#define USEPQUEUE 
//#define BLMALGO 
//#define RECURSIVEALGO 
/* ---------------------------------------------------------------------------------------------------------------------------------------- */



/* Activate the following #defines to use the eTM Algorithm for Historical Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-5
//#define EPSCPL 1.0e-7
//#define LINEGENDELTA 1000000

//#define RESTRICTIVELINE
/* ---------------------------------------------------------------------------------------------------------------------------------------- */

/* Activate the following #defines to use the eTM Algorithm for Rand and Bent Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-4
//#define EPSCPL 1.0e-6
//#define LINEGENDELTA 100000

//#define RESTRICTIVELINE
/* ---------------------------------------------------------------------------------------------------------------------------------------- */



/* Activate the following #defines to use the PureLex Algorithm for Historical Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-5
//#define EPSCPL 1.0e-7
//#define LINEGENDELTA 1000000

//#define USEPQUEUE 
//#define IMPROVEMENTNOGOOD
//#define BLMALGO 
//#define RESTRICTIVELINE
/* ---------------------------------------------------------------------------------------------------------------------------------------- */

/* Activate the following #defines to use the PureLex Algorithm for Rand and Bent Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-4
//#define EPSCPL 1.0e-6
//#define LINEGENDELTA 100000

//#define USEPQUEUE 
//#define BLMALGO 
//#define RESTRICTIVELINE
/* ---------------------------------------------------------------------------------------------------------------------------------------- */



/* Activate the following #defines to use the SPureLex-0.05 Algorithm for Historical Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-5
//#define EPSCPL 1.0e-7
//#define LINEGENDELTA 1000000

//#define USEPQUEUE 
//#define IMPROVEMENTNOGOOD
//#define BLMALGO 
//#define RESTRICTIVELINE

//#define RESTRICTIVELINEALGORITHMSWITCHTOETM
//#define RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM 95
/* ---------------------------------------------------------------------------------------------------------------------------------------- */

/* Activate the following #defines to use the SPureLex-0.05 Algorithm for Rand and Bent Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-4
//#define EPSCPL 1.0e-6
//#define LINEGENDELTA 100000

//#define USEPQUEUE 
//#define BLMALGO
//#define RESTRICTIVELINE

//#define RESTRICTIVELINEALGORITHMSWITCHTOETM
//#define RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM 95
/* ---------------------------------------------------------------------------------------------------------------------------------------- */


/* Activate the following #defines to use the SPureLex-0.005 Algorithm for Historical Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-5
//#define EPSCPL 1.0e-7
//#define LINEGENDELTA 1000000

//#define USEPQUEUE 
//#define IMPROVEMENTNOGOOD
//#define BLMALGO
//#define RESTRICTIVELINE

//#define RESTRICTIVELINEALGORITHMSWITCHTOETM
//#define RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM 99.5
/* ---------------------------------------------------------------------------------------------------------------------------------------- */


/* Activate the following #defines to use the SPureLex-0.005 Algorithm for Rand and Bent Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-4
//#define EPSCPL 1.0e-6
//#define LINEGENDELTA 100000

//#define USEPQUEUE
//#define BLMALGO 
//#define RESTRICTIVELINE

//#define RESTRICTIVELINEALGORITHMSWITCHTOETM
//#define RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM 99.5
/* ---------------------------------------------------------------------------------------------------------------------------------------- */


/* Activate the following #defines to use the eTM-PWL Algorithm for Historical Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-5
//#define EPSCPL 1.0e-7
//#define LINEGENDELTA 1000000

//#define RESTRICTIVELINE
//#define PWL
/* ---------------------------------------------------------------------------------------------------------------------------------------- */

/* Activate the following #defines to use the eTM-PWL Algorithm for Rand and Bent Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-4
//#define EPSCPL 1.0e-6
//#define LINEGENDELTA 100000

//#define RESTRICTIVELINE
//#define PWL
/* ---------------------------------------------------------------------------------------------------------------------------------------- */


/* Activate the following #defines to use the SPureLex-PWL-0.05 Algorithm for Historical Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-5
//#define EPSCPL 1.0e-7
//#define LINEGENDELTA 1000000

//#define USEPQUEUE 
//#define IMPROVEMENTNOGOOD
//#define BLMALGO 
//#define RESTRICTIVELINE
//#define PWL

//#define RESTRICTIVELINEALGORITHMSWITCHTOETM
//#define RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM 95
/* ---------------------------------------------------------------------------------------------------------------------------------------- */

/* Activate the following #defines to use the SPureLex-PWL-0.05 Algorithm for Rand and Bent Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-4
//#define EPSCPL 1.0e-6
//#define LINEGENDELTA 100000

//#define USEPQUEUE 
//#define BLMALGO 
//#define RESTRICTIVELINE
//#define PWL

//#define RESTRICTIVELINEALGORITHMSWITCHTOETM
//#define RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM 95
/* ---------------------------------------------------------------------------------------------------------------------------------------- */


/* Activate the following #defines to use the SPureLex-PWL-0.005 Algorithm for Historical Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-5
//#define EPSCPL 1.0e-7
//#define LINEGENDELTA 1000000

//#define USEPQUEUE 
//#define IMPROVEMENTNOGOOD
//#define BLMALGO 
//#define RESTRICTIVELINE
//#define PWL

//#define RESTRICTIVELINEALGORITHMSWITCHTOETM
//#define RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM 99.5
/* ---------------------------------------------------------------------------------------------------------------------------------------- */

/* Activate the following #defines to use the SPureLex-PWL-0.005 Algorithm for Rand and Bent Instances (and comment all the other remaining #defines) */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
//#define EPS 1.0e-4
//#define EPSCPL 1.0e-6
//#define LINEGENDELTA 100000

//#define USEPQUEUE 
//#define BLMALGO 
//#define RESTRICTIVELINE
//#define PWL

//#define RESTRICTIVELINEALGORITHMSWITCHTOETM
//#define RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM 99.5
/* ---------------------------------------------------------------------------------------------------------------------------------------- */

#endif