

									//  or probe ice following because knows of hostility
#define STATE_
#define STATE_



when player attacks,
	set all black ice in node to attacking
	set probe ice to attacking (actually following)
	set gate/tape/guardian as guardig-hostile
	if attack tapeworm, set to destroying

when player leaves node
	if ice is guarding hostile, goes to guarding normal
	all others stay same

on ice's turn
	inactive - nothing
	guarding - if accessed, query player
	guarding h - nothing
	following - if in same node, query
				if not in same node, chase
	moving - if in same node, query
			otherwise, move towards target
	searching - if in same node, query
				otherwise move randomly
	queried - if deceived, go to guarding
				if not, raise alarm, 
					if red alert, go to guarding (hostile if in node)
					else query again or guard
				black ice behaves differently
	destroying - destroy file and self
	attacking - same node - attack
				near node - chase
				not near - search

	