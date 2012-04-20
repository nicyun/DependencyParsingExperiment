#ifndef _PARAMETER_H_
#define _PARAMETER_H_

/*global variables*/

#define LEARNTIMES	20	/*times of online learning*/
#define BETA		0.2	/*beta for mutation*/
#define DETA		0.1	/*quantity changed by mutation*/

/*Status of Word-agent*/
#define ACTIVE		1	/*active*/
#define MATCH		2	/*match*/
#define MUTATE		3	/*mutate*/
#define CLONE		4	/*clone*/
#define MATURE		5	/*mature*/
#define DIE		6	/*die*/

#define AFFINITY	0.8	/*affinity threshold for adaptive recognition*/
#define STIMULUS	0.0	/*stimulus threshold*/
#define SUPPRESS	0.0	/*suppress threshold*/

#endif
