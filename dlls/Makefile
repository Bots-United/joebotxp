CPP = gcc
BASEFLAGS = -Dstricmp=strcasecmp -Dstrcmpi=strcasecmp
CPPFLAGS = ${BASEFLAGS} -w -march=i686 -O2 -I"../common/" -I"../engine/" -I"../pm_shared/" -Wall

OBJ = 	AStarBase.cpp AStarBaseGoal.cpp AStarGoalHide.cpp AStarGoalMod.cpp AStarGoalPFled.cpp AStarMachine.cpp AStarNode.cpp AStarNodeContainer.cpp AStarTaskManager.cpp Action.cpp BaseBot.cpp BaseSquad.cpp BaseTeam.cpp Behaviour.cpp BitField.cpp BotCS.cpp BotDOD.cpp BotHLDM.cpp Command.cpp Commandfunc.cpp Emotion.cpp Entity.cpp Game.cpp GameCS.cpp GameDOD.cpp GameHL.cpp GameHLDM.cpp Goal.cpp GoalFinder.cpp HPTime.cpp InterestingEntity.cpp Log.cpp Map.cpp Names.cpp Observer.cpp ObserverItem.cpp PQNearWaypoint.cpp Path.cpp Perceipt.cpp Perception.cpp PerceptionBot.cpp PerceptionSquad.cpp Player.cpp PlayerCS.cpp Profiler.cpp Radio.cpp RespCurve.cpp TeamCS.cpp WPStat.cpp Way.cpp Waypoint.cpp WaypointGraph.cpp Weapons.cpp WeaponsCS.cpp WeaponsDOD.cpp WeaponsHLDM.cpp bot.cpp bot_client.cpp dll.cpp engine.cpp h_export.cpp linkfunc.cpp sbv_hldm.cpp util.cpp

joebot_i686.so: ${OBJ}
	${CPP} ${CPPFLAGS} -fPIC -shared -o $@ ${OBJ} -Xlinker -Map -Xlinker joebot.map -ldl -lstdc++

clean:
	-rm -f *.o
	-rm -f *.so	

%.o:	%.cpp
	${CPP} ${CPPFLAGS} -c $< -o $@

%.o:	%.c
	${CPP} ${CPPFLAGS} -c $< -o $@

