#pragma once
//These Function chunks were imported from DearImgui's Demo and .h files.
//I honestly do not know which items in particular are absolutely required for the functional
//portions of the program. I will trim this down in time, hopefully.

// Configuration file (edit imconfig.h or define IMGUI_USER_CONFIG to set your own filename)
#ifdef IMGUI_USER_CONFIG
#include IMGUI_USER_CONFIG
#endif
#if !defined(IMGUI_DISABLE_INCLUDE_IMCONFIG_H) || defined(IMGUI_INCLUDE_IMCONFIG_H)
#include "imconfig.h"

#endif

#include <float.h>          // FLT_MAX
#include <stdarg.h>         // va_list
#include <stddef.h>         // ptrdiff_t, NULL
#include <string.h>         // memset, memmove, memcpy, strlen, strchr, strcpy, strcmp
#include <math.h>
#include <stdlib.h>
#include <ctime>
#include <string.h>
//#include <imgui.cpp>

#define IMGUI_VERSION       "1.60 WIP"

// Define attributes of all API symbols declarations, e.g. for DLL under Windows.
#ifndef IMGUI_API
#define IMGUI_API
#endif

// Define assertion handler.
#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR)    assert(_EXPR)
#endif

// Helpers
#if defined(__clang__) || defined(__GNUC__)
#define IM_FMTARGS(FMT)             __attribute__((format(printf, FMT, FMT+1))) // Apply printf-style warnings to user functions.
#define IM_FMTLIST(FMT)             __attribute__((format(printf, FMT, 0)))
#else
#define IM_FMTARGS(FMT)
#define IM_FMTLIST(FMT)
#endif
#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))         // Size of a static C-style array. Don't use on pointers!
#define IM_OFFSETOF(_TYPE,_MEMBER)  ((size_t)&(((_TYPE*)0)->_MEMBER))           // Offset of _MEMBER within _TYPE. Standardized as offsetof() in modern C++.

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif

//static bool a = false;
//static bool b = false;

void Popups()
{
	//This is PROOF that I can have a sub-tree within the Demo INDEPENDENT 
	//From the main demo.cpp.
	//Popups has been modified from the demo as a way to make sure things worked.
	//This Function is DEFUNCT, but if you wish, throw it into the main to see how the interface interacts.
	if (ImGui::TreeNode("Popups"))
	{
		ImGui::TextWrapped("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.");

		static int selected_fish = -1;
		const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
		static bool toggles[] = { true, false, false, false, false };

		// Simple selection popup
		// (If you want to show the current selection inside the Button itself, you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
		if (ImGui::Button("Select.."))
			ImGui::OpenPopup("select");
		ImGui::SameLine();
		ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
		if (ImGui::BeginPopup("select"))
		{
			ImGui::Text("Aquarium");
			ImGui::Separator();
			for (int i = 0; i < IM_ARRAYSIZE(names); i++)
				if (ImGui::Selectable(names[i]))
					selected_fish = i;
			ImGui::EndPopup();
		}

		// Showing a menu with toggles
		if (ImGui::Button("Toggle.."))
			ImGui::OpenPopup("toggle");
		if (ImGui::BeginPopup("toggle"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(names); i++)
				ImGui::MenuItem(names[i], "", &toggles[i]);
			if (ImGui::BeginMenu("Sub-menu"))
			{
				ImGui::MenuItem("Click me");
				ImGui::EndMenu();
			}

			ImGui::Separator();
			ImGui::Text("Tooltip here");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("I am a tooltip over a popup");

			if (ImGui::Button("Stacked Popup"))
				ImGui::OpenPopup("another popup");
			if (ImGui::BeginPopup("another popup"))
			{
				for (int i = 0; i < IM_ARRAYSIZE(names); i++)
					ImGui::MenuItem(names[i], "", &toggles[i]);
				if (ImGui::BeginMenu("Sub-menu"))
				{
					ImGui::MenuItem("Click me");
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Popup Menu.."))
			ImGui::OpenPopup("FilePopup");

		ImGui::TreePop();
	}


};

//IMPORTED DEFINITTIONS FROM IMGUI DEMO FOR CONVENIENCE's SAKE
static void GURPS_ShowHelpMarker(const char* desc)
{
	//A Direct Conversion of the ImGui Demo's "ShowHelpMarker. Re-created here for convenience's sake. 
	ImGui::TextDisabled("(Info)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}



//FUNCTIONS FOR THE GURPS APPLICATION:

int rollSingleDie()
{
	//I hope that I may find a more reliable or less hacky solution to random numbers, 
	//But this perfoms better than I had originally expected.
	//This function works best if you roll less than once per second.

	int d = (rand() % 6 + 1);

	return d;
}

int rollMultipleDie(int n)
{
	//Generic function for rolling any number of die, and return its values. 

	int d = 0;
	//int d = 0;
	//srand(time(NULL)); //This function assumes that Srand Time is called within the function it is called in.
	//sleep();
	for (int i = 0; i < n; i++)
	{
		d = d + (rand() % 6 + 1);
	}
	return d;
}





int success(int skill, int modifier, int roll)
{
	// Handles the sucess or failiure of the roll without any strings attached to DearImGui.
	// 
	// 1 is Critical Failiure, 4 is Critical Success, and 2 and 3 are failiure and success, respectively.
	//
	int result = 0;

	//int degree = ((skill + modifier) - roll);
	int degree = ((skill + modifier) - roll);

	if ((roll == 3 || roll == 4))
	{
		result = 4;
		return result;
	}
	else if ((roll == 5 && skill >= 15) || (roll == 6 && skill >= 16))
	{
		result = 4;
		return result;
	}

	//CRITICAL FAILIURE HANDLING
	else if (roll == 17)
	{
		if (skill <= 15)
		{
			result = 1;
			return result;
		}
		else
		{
			result = 2;
			return result;
		}
	}
	else if (roll == 18 || (degree <= -10))
	{
		result = 1;
		return result;
	}
	//SUCESS HANDLING
	else if (degree >= 0)
	{
		result = 3;
	}
	else if (degree < 0)
	{
		result = 2;
	}
	return result;
	//CRITICAL SUCCESS HANDLING

}

int DamageTableDie(int str,const char* type)
{
	//The Die Portion of the Melee damage Table. 
	//MAY BE A TAD BUGGY
	if (type == "Swing") 
	{
		if (str >= 1 && str <= 12)
		{
			return 1;
		}
		if (str >= 13 && str <= 16) 
		{
			return 2;
		}
		else if (str >= 17 && str <= 20)
		{
			return 3;
		}
	}
	else if (type == "Thrust") 
	{
		if (str >= 1 && str <= 18)
		{
			return 1;
		}
		if (str >= 19 && str <= 20)
		{
			return 2;
		}
	}
	return 1;
	/*
	if (str == 1 || str == 2)
	{

	}
	*/
}

int DamageTableMod(int str,const char* type)
{
	//The Modifier Portion of the Melee damage Table: 
	//This is a brute force method of doing so, 
	//If we had more time, perhaps an array would work a lot more cleanly.
	//MAY BE A TAD BUGGY
	if (type == "Swing")
	{
		if (str >= 1 && str <= 2)
		{
			return -5;
		}
		else if (str >= 3 && str <= 4)
		{
			return -4;
		}
		else if (str >= 5 && str <= 6)
		{
			return -3;
		}
		else if (str >= 7 && str <= 8)
		{
			return -2;
		}
		else if (str == 9 || str == 13 || str == 17)
		{
			return -1;
			//NOT DONE WITH THIS YET
		}
		else if (str == 10)
		{
			return 0;
		}
		else if (str == 11 || str == 15 || str == 19)
		{
			return 1;
		}
		else if (str == 12 || str == 16 || str == 20)
		{
			return 2;
		}
	}
	else if (type == "Thrust" )
	{
		if (str >= 1 && str <= 2)
		{
			return -6;
		}
		else if (str >= 3 && str <= 4)
		{
			return -5;
		}
		else if (str >= 5 && str <= 6)
		{
			return -4;
		}
		else if (str >= 7 && str <= 8)
		{
			return -3;
		}
		else if (str >= 9 && str <= 10)
		{
			return -2;
		}
		else if ((str >= 11 && str <= 12) || (str >= 19 && str <= 20))
		{
			return -1;
		}
		else if (str >= 13 && str <= 14)
		{
			return 0;
		}
		else if (str >= 15 && str <= 16)
		{
			return 1;
		}
		else if (str >= 17 && str <= 18)
		{
			return 2;
		}
	}
	return 0;
}

//DearIMGUI ORITENTED CONTENTS



void successRollAddInfo()
{
	//Additional info for the Success Roll Helper. 
	//Things otherwise would be rather cluttered in the page.
	ImGui::Text("\n          |__________ADDITIONAL_INFORMATION__________|\n");
	if (ImGui::TreeNode("For more information on other uses for Success rolls, Select here.\nSuccess Rolls are the Heart of GURPS. Be sure to familiarize yourself."))
	{
		/*
		This is an 80Column set of dots. Nice and clean.
		We will use this to better type-set the rest of the writing.
		...............................................................................

		*/
		ImGui::Text("\n");
		if (ImGui::TreeNode("What is a Success Roll?:"))
		{

			ImGui::Text("Success Rolls are the core of GURPS. If there's a chance of dire failure,\nor rewarding success, a Success Roll is the deciding line of such a fate!\n\nThe Success Roll rolls 3 die, against the effective skill in question.\nWhen the totaled roll is BELOW the effective skill, the roll succeeds.\nNo matter how high or low an effective skill is; rolling 3 or 4 succeeds.\nA roll of 17 or 18 always fails, no matter how skilled one may be.\n\nThe Degree of Success or Failure is result of your roll minus your skill.\nThis is often used to break ties, or to affect very good or poor rolls.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");

		if (ImGui::TreeNode("Contest Roll Information:"))
		{
			ImGui::Text("In a Quick Contest(ex:Two adventurers lunging for the same shiny artifact);\nContestants will make a Success Roll including their effective skills. \nThe highest roll of the two contestants wins the contest.\nThe contestant with the highest degree of success breaks a tie.\nIf both parties have identical degrees of success, no one wins.");
			//ImGui::Text("\nRegular Contests(ex:Two politicians at a televised debate);\nAre similar to Quick Contests, but,\nif both parties succeed or fail, \nthey re-roll until one loses and the other wins. \n\nIf both contestants constantly have scored of 6 or less, \nraise the lower score to 10. Next, \nadd that amount to the higher roll and evaluate the winner.\nIf the two contestant's scores are higher than 14, to prevent stalemates, complete the opposite process. \n");
			ImGui::Text("\nRegular Contests(ex:Two politicians at a televised debate);\nAre similar to Quick Contests, but if both parties succeed,\nor both parties fail simultaneously, they re-roll.\nThis continues until one contestant loses and the other wins. \nSee Page 349 of GURPS: Campaigns for information on extreme tiebreakers.\n\n");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		//ImGui::TreePop();
		if (ImGui::TreeNode("Resistance Roll Information:"))
		{
			//ImGui::Text("");
			ImGui::Text("In a Resistance Roll, an unwilling subject rolls to resist an effect\nby rolling a Success Roll with the necessary skill or attribute.\n(Ex:A Will Roll against the Alien as it attempts to influence your mind.)\n\nThe attacker must win outright in order to affect their target.\nIn the event of a tie, the defender succeeds, and is left unharmed.\n\nIf the attack is supernatural in nature against a living or sapient being, \nthe attacker's effective skill can't exceed 16 unless the defender's\nresistance is greater than 16. If so, the defender rolls the larger value.\nMost Resistance rolls are Will Rolls(By Default, your IQ)");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		if (ImGui::TreeNode("Sense Roll Information:"))
		{
			ImGui::Text("Sense Rolls involve a roll against your senses to detect something. \nIf a Sense Roll succeeds, the detail should be very important!\nIf successful, you may need a second roll to understand it in detail.\n(Ex:The queen's wine tastes tinny at her coronation feast.\nMake a Comprehension Roll to determine if it's actually poison.)\nTo roll, use your Perception(By Default, your IQ) as the governing skill.\n\nItems visually hidden(Ex:A Stowaway) can be treated as a Quick Contest \nagainst the person hiding themselves or an object. (Holdout, Camouflage).\n\nFor situations that test hearing, make another IQ Roll to make out speech.\nIn a Quick Contest role to stay silent, use the Stealth Skill.\nSee Page 358 of the Basic Set: Campaigns for hearing distances.\n\nTaste and Smell are similar. The advantages Discriminatory Taste and Smell \ncan allow you to identify people, places, and things similar to vision.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		ImGui::TreePop();
	}


};




void successRoll()
{
	//This function handles Success Rolls and is called in the Main program.
	if (ImGui::CollapsingHeader("\nSuccess Roll Handler\n\n"))
	{
		srand(time(NULL));
		static bool is_CombatRoll = false;
		static bool is_ATK = false;
		static bool is_DEF = false;
		static bool isSucRollIO = false;
		static bool isManualSum = false;
		const char* manuverArrayATK[] = { "All-Out Attack:Melee", "Thrust" };
		const char* manuverArrayDEF[] = { "All out Defense", "Thrust" };
		//
		static int skill = 10;
		static int modifier0 = 0;
		static int roll = 10;
		static int die1 = 3;
		static int die2 = 4;
		static int die3 = 3;
		static int degree = 0;
		int result = 3;

		ImGui::Text("Roll 3 Die against your skills, attributes, and situational modifiers. \nThis can be used for Attribute Rolls(ex:Will Rolls), Skill Rolls, \nand can help you determine if you succeed under pressure.\n\n\nFor additional details See Page 343-349 of Gurps 4e Basic Set: Campaigns");

		//ImGui::Text("For more information on other uses for Success rolls, Select here.");
		successRollAddInfo();
		//to preserve the clarity of the function, I'm moving the texty bits into seperate functions.

		//
		ImGui::Text("\n__________SUCCESS ROLL INPUT_______________________\n\n");

		////

		/*
		ImGui::Checkbox("Apply Modifiers for Combat Manuvers: Attack and Defense", &is_CombatRoll);
		if (is_CombatRoll == true)
		{
		int selected_manuver = -1;
		//

		//STILL NEED TO FINISH THE MELEE MANUVERS HERE FIRST.

		//
		if (ImGui::Button("Select Combat Manuver Modifier\n\n"))
		ImGui::OpenPopup("selectManuver");
		ImGui::SameLine();
		//NEED TO HAVE MULTIPLE SELECTABLE ARRAYS WITH ATTEMPTS AT SOME
		//WAY TO SIMPLIFY CONVERSION OF TEXT TO VALUES IN A 2D ARRAY?
		ImGui::TextUnformatted(selected_manuver == -1 ? "<None Selected>" : manuverArray[selected_manuver]);
		ImGui::SameLine(); GURPS_ShowHelpMarker("Use this to quickly apply bonuses or penalties \nfrom a fine selection of Combat Manuvers. \n");

		if (ImGui::BeginPopup("selectManuver"))
		{
		ImGui::Text("Selected Manuver:");
		ImGui::Separator();
		for (int k = 0; k < IM_ARRAYSIZE(manuverArray); k++)
		if (ImGui::Selectable(manuverArray[k]))
		selected_manuver = k;

		ImGui::EndPopup();
		}

		}
		*/


		//Primary Controls for Success Roll:
		//ImGui::SliderInt("Skill", &skill, 0, 20); ImGui::SameLine(150);

		ImGui::Checkbox("Collapse Success Roll Input?", &isSucRollIO);
		ImGui::SameLine(); GURPS_ShowHelpMarker("Select to expand the Success Roll Input Area. Collapse if you feel the workspace is too cluttered.");
		if (isSucRollIO == false)
		{
			ImGui::Text("\n\n");
			ImGui::SliderInt("Skill", &skill, 0, 20); //ImGui::SameLine(50);
			ImGui::SameLine(); GURPS_ShowHelpMarker("Slide in the highest skill or attribute to roll against. \nIf you are rolling against an Attribute(HT,IQ,ST) more that 20, treat it as 20.");
			ImGui::SliderInt("Skill Modifier(s)", &modifier0, -10, 10);
			ImGui::SameLine(); GURPS_ShowHelpMarker("In certain situations, your chance of success\ncan be more or less favorable. \n\nThis modifier fine tunes your effective skill,\nwhich will modify your final rolled value.\n(Ex: -5 to your Running skill as your\nfoe has greased your escape path) \n\nAll Equipment and situational\nmodifiers can be tallied up here.");

			ImGui::Checkbox("Select to manually enter Roll Value.", &isManualSum);
			ImGui::SameLine(); GURPS_ShowHelpMarker("Select to slide in the roll value you summed up.\nGreat for when you wish to test the degree of success.");
			if (isManualSum == false) 
			{
				//If you want to change the values of the roll directly, enable the slider below for debug.
				//ImGui::SliderInt("DEBUG: RollValue", &roll, 3, 18);
				ImGui::SameLine(); ImGui::Text("          ");
				if (ImGui::Button("\n     Click to Roll   \n\n"))
				{
					die1 = rollSingleDie();
					//srand(time(NULL));
					die2 = rollSingleDie();
					die3 = rollSingleDie();
					roll = (die1 + die2 + die3);
				}



				
				//ImGui::SameLine(300);
				//ImGui::SameLine(); ImGui::Text("\nCurrent Roll is 3d+ %d\n", modifier0);
				ImGui::SameLine(); ImGui::Text("\nYou will roll: 3d + %d\n", modifier0);
				//3d + %d
			}
			else 
			{
				ImGui::SliderInt("Roll Value", &roll, 3, 18);
			}
			degree = ((skill + modifier0) - roll);
			result = success(skill, modifier0, roll);

			

		}
		else
		{
			ImGui::Text("Success Roll Input is Collapsed.\n");
		}




		ImGui::Text("\n          |__________SUCCESS ROLL RESULTS__________|");
		ImGui::Separator();
		ImGui::Text("          3-Die Output: %d, %d, %d", die1,die2,die3);
		ImGui::Text("          Sum of 3d Roll: %d", roll);
		ImGui::Text("          \n          Effective Skill: %d", (roll + modifier0));
		ImGui::Text("          \n          Result of Success Roll:");
		if (result == 1)
		{
			ImGui::Text("          Disaster! You Critically Failed this roll.\n          The Degree of Success or Failure was: %d", degree);
		}
		else if (result == 2)
		{
			ImGui::Text("          Ouch! You Failed this roll.\n          The Degree of Success or Failiure was: %d", degree);
		}
		else if (result == 3)
		{
			ImGui::Text("          Good! You Succeeded in this roll.\n          The Degree of Success or Failure was: %d", degree);
		}
		else if (result == 4)
		{
			ImGui::Text("          Great! You Critically Succeeded in this roll.\n          The Degree of Success or Failure was: %d", degree);
		}
		ImGui::Text("\n\n");
	}
};






void reactionRolls()
{

	if (ImGui::CollapsingHeader("\nReaction Roll Handler \n\n"))
	{
		//Swiftly and clearly handles reaction rolls. 
		static int modifier2 = 0;
		static int reaction = 10;
		//default is 10, the average reaction.


		ImGui::Text("Roll 3 Die to determine the impressions of the NPC(s) to the PC(s). \n See Page 560 of GUPRS 4e Basic Set: Campaigns");
		ImGui::SliderInt("Modifier(Rec)", &modifier2, -10, 10);
		ImGui::SameLine(); GURPS_ShowHelpMarker("To apply other biases of the NPC(s) involved,\nuse the slider to give advantage or disadvantage to the roll. \n most advantages or disadvantages are from 1 to 3.");
		ImGui::Text("__________REACTION ROLL INPUT_______________________\n");
		if (ImGui::Button("\n          Roll 3d to Make Reaction Roll.\n\n"))
			reaction = rollMultipleDie(3) + modifier2;

		ImGui::Text("Current Reaction Roll: %d\n", reaction);
		//ImGui::Text("Textual Results of Reaction Roll: %s", toolText);
		ImGui::Text("RESULTS");
		ImGui::Separator();
		ImGui::Text("Textual Results of Reaction Roll:");
		//This could REALLY be improved by an array of strings, and only evaluated per selection of the roll.
		if (reaction <= 0)
		{
			ImGui::Text("The NPC hates the characters and will act in their worst interest. \n Nothing is out of the question; assault, betrayal, public ridicule, \n or ignoring a life-or-death plea are all possible.");
		}
		else if (reaction >= 1 && reaction <= 3)
		{
			ImGui::Text("The NPC dislikes the characters and will act against them \nif it is convenient to do so. This can include attacking the party, \noffering grossly unfair terms in a transaction, and so on.");
		}
		else if (reaction >= 4 && reaction <= 6)
		{
			ImGui::Text("The NPC cares nothing for the characters and will act against them \nto misinform or endanger the party, if he/she can benefit by doing so.");
		}
		else if (reaction >= 7 && reaction <= 9)
		{
			ImGui::Text("The NPC is unimpressed. He/she may make threats, demand a bribe \n before offering aid, or something similar. Miffed, but not furious.");
		}
		else if (reaction >= 10 && reaction <= 12)
		{
			ImGui::Text("The NPC ignores the characters as much as possible. \nHe/she is totally uninterested in the fate of the party.\nTransactions will go smoothly and routinely, as long as protocol is observed.");
		}
		else if (reaction >= 13 && reaction <= 15)
		{
			ImGui::Text("The NPC likes the characters and will be helpful within normal, everyday limits. \nReasonable requests will be granted.");
		}
		else if (reaction >= 16 && reaction <= 18)
		{
			ImGui::Text("The NPC thinks highly of the characters and will be quite helpful and friendly, \n freely offering aid and favorable terms in most things.");
		}
		else if (reaction >= 19)
		{
			ImGui::Text("The NPC is extremely impressed by the characters, and will act in their best \n interests at all times, within the limits of his/her own ability \n – perhaps even risking his life, wealth, or reputation.");
		}
		ImGui::Text("\n\nRemember: Never substitute these random Dice rolls for basic reason and logic.");

		ImGui::Text("\n\n");
	}
};



void damageRollAddInfo()
{
	ImGui::Text("\n          |__________ADDITIONAL INFORMATION__________|\n");
	if (ImGui::TreeNode("For more information regarding Combat and sustaining Injury, Select here.\n This information just might save your PC."))
	{
		ImGui::Text("\n");
		if (ImGui::TreeNode("Basic Combat Flow and Turn sequence:"))
		{
			ImGui::Text("Combat in GURPS takes place each turn by a span a second or so, for detailed one on one combat. \nLonger timeframes are more suitable for other situations, like mass combat.\nTurn order is determined by each PC/NPC's Basic Speed. PC ties are broken by highest DX.\nIf a tie is still in the way of turn progress, it is broken by the GM randomly.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		
		//ImGui::TreePop();
		if (ImGui::TreeNode("Attack Flow During A Turn:"))
		{
			//ImGui::Text("");
			ImGui::Text("Here is the Basic Flow of an Attack within One Turn of Combat:");
			ImGui::BulletText("Make an attack roll(a Success Roll); rolling against your effective skills with your weapon.\nA Critical Success with an Attack Roll skips a defender's Defense Roll.");
			ImGui::BulletText("If your attack is successful, your opponent must make a Defense roll to evade it.\nThis Defensive Roll(A Success Roll) is against the defender's active defense score.\nFor More Info, refer to the Tab on Defense.");
			ImGui::BulletText("If you succeed in attempting to hit your target, and they fail to defend against it,\nyour attack hits home, and you will Roll for Damage.");
			ImGui::TreePop();
		}

		//We Could REALLY Use an entry on How to Handle and evaluate your on defenses for the defense roll HERE.
		//It's somewhat optional. it wouldn't break my heart but it would make things go smoothly.
		//Page 374 of the Basic Set should give you everything you need.
		ImGui::Text("\n");
		if (ImGui::TreeNode("What to Do During a Turn: Maneuvers"))
		{
			ImGui::Text("During your turn, you can perform a variety of Maneuvers. Whatever Maneuver you choose in your turn \nwill determine what you can do on your turn, and open or close options for attack or defense.\nGeneral Descriptions of some Maneuvers are below. for more details, see the Upcoming Combat Status Tool.");
			ImGui::BulletText("Do Nothing:\nTake any Active Defense, but No movement.");
			ImGui::BulletText("Attack:\nMake an armed attack, melee or ranged. Your weapons must be ready, and target in reach.");
			ImGui::BulletText("All Out Attack:\n Make an armed attack, with no effort to defend yourself.\nVariations of the All Out Attack are covered in the Combat Status Tool.");
			ImGui::BulletText("All Out Defense:\nIncrease your defense options at the cost of attack opportunity. \nDefensive moves vary, and are referenced in the Combat Status Tool.");
			ImGui::BulletText("Move:\nMove any number of hexes/yards, up to your maximum Move Distance, and take any Active Defense.\n");
			//If you run forward for 2+Turns, you receive a 20 percent Move Bonus rounded down.
			ImGui::BulletText("Change Posture:\nSwitch between one posture to another. You cannot move while changing posture.\nMore detailed info on these postures are in the Combat Status Tool");
			ImGui::BulletText("Ready:\nGrab or draw any Item and prepare it for use. Some weapons need a Ready Maneuver to be re-used.\nReady maneuvers can be used to operate physical items during combat. \nTake any active defense, but movement is limited to a Step.");
			ImGui::BulletText("Aim:\nWith specified weapon and target, spend the turn aiming at them. \nIf Aim is followed by an attack, add your weapon's accuracy to your effective skill. \nInjury while aiming needs a Will Roll to check if you lost your aim.\nTake any active defense at the cost of your aim. You can take a step as well.\nSimilar Maneuvers like Evaluate, Concentrate, and Wait are in the Upcoming Combat Status Tool.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		if (ImGui::TreeNode("Free Actions:"))
		{
			ImGui::Text("Free Actions can be completed during any maneuver. These include:");
			ImGui::BulletText("Speech:\nNo matter what, you can always talk in a maneuver. A sentence or two per turn is reasonable.");
			ImGui::BulletText("Maintain Supernatural Ability:\nAs long as you are active, you can maintain a spell or psionic ability.");
			ImGui::BulletText("Drop a Ready item:\nYou can drop any item that you have readied. You can drop the item anywhere in reach.");
			ImGui::BulletText("Crouch:\n If possible, you can crouch (or rise from a crouch to stand) at the start of your turn.\nMovement will be slowed, and you can't sprint, but you will be a harder target to hit.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		if (ImGui::TreeNode("Injury and You: When the Dog Bites/Bee Stings."))
		{

			ImGui::Text("Your HP is determined by your Strength, and can be augmented by spending character points.\nYou will take damage if your opponent's attack penetrates completely through your DR.\nThe Penetrated damage is then modified by the weapon's wounding modifier.\n\nAny penetrated attack is devastating to you or an NPC's combat effectiveness.\n");
			if (ImGui::TreeNode("Taking Injury:\n\n"))
			{
				ImGui::BulletText("If you have less than 1/3 of your HP, you're badly wounded, Move and Dodge are halved.\n\n");

				ImGui::BulletText("If you have lost HP during a Turn, you are briefly in Shock for your next turn.\nYour DX and IQ for the next turn will be reduced in equal measure to HP loss, up to -4.\nShock doesn't penalize active defensive maneuvers, but it will cripple other skills briefly.\n\n");
				ImGui::TreePop();
			}
			//ImGui::Text("Crippling Injury:");
			if (ImGui::TreeNode("Crippling Injury:\n\n"))
			{
				ImGui::BulletText("If you use hit locations, injury to a limb past a certain HP threshold cripples it.\nArms and Legs are crippled past a victim's HP/2, Heads/Hands at HP/3, and Eyes at HP/10.\nIf you have more limbs than usual, their crippling threshold is HP/(1.5 * #of Limbs).\n\n");
				ImGui::BulletText("A limb injury will never cause HP damage past the minimum HP to cripple it.\nFor example, if a character has 10HP, and their foot is crushed by an Anvil for 6 damage, \nthe victim only loses 4HP, but the victim's foot is injured almost beyond repair.\nThere is a grim exception to this rule. Eye injuries do NOT have a damage limit.\nIf a body part is injured beyond TWICE the crippling threshold, it is gone forever.\n\n");
				ImGui::BulletText("If you have received a crippling injury, make an HT roll at the end of Combat.\nIf the the HT roll Succeeds, your injury is Temporary.\nIf you fail, your injury is Lasting, and will take a long time to heal.\nIf you have critically failed, it's Permanent. You lose function of the body part forever.\nIf your Crippling injury is temporary, you suffer its penalties until your HP is full.\nIf Lasting, roll 3d for the number of months it takes to fully heal outside of treatment.\n Permanent crippling gives you a new disadvantage with no character point bonuses.\n\n");
				ImGui::TreePop();
			}
			//ImGui::BulletText("\n\n");
			//ImGui::Text("");
			//ImGui::Text("Major Wounds:");
			if (ImGui::TreeNode("Major Wounds:\n\n"))
			{
				ImGui::BulletText("If you sustain a single injury more than 1/2 of your HP in most situations,\nor a body part is Crippled, you have a Major Wound, and you are in trouble.\nTo avoid knockdown and stun from such a grave injury, Take an HT Roll. \nThe HT roll is taken at HT-5 for injuries to the face, Male groin, or vitals. \nRoll to avoid knockdown at HT-10 if wounded in the Skull or Eye.\nRoll to avoid knockdown at HT+3 and HT-4 for High and Low Pain Threshold, respectively.\nIf the HT roll failed, you are stunned. Fall prone and drop anything you were carrying.\nWith a degree of failure greater than 5, you fall unconscious. When stunned, you can take Defense Rolls at -4.\nTo recover from a stun, roll against HT. If successful, you recover. If not, Do Nothing,\n but you can attempt another HT roll to recover on your next turn.\n\n");

				ImGui::BulletText("if you have 0 or less HP, you are in danger of Collapse.\nOn your next turn, make an HT roll at -1 of every full multiple of HP below 0.\nIf Successful, you proceed as usual, but you must continue succeeding to stay in battle.\nIf you Do Nothing, you can focus on remaining conscious, and won't need to roll.\nIf you fail the HT roll, you fall unconscious.\n\n");
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		ImGui::Text("\n");
		if (ImGui::TreeNode("Mortal Injury: Knocking on Heaven's Door."))
		{
			ImGui::Text("If you are here, you have suffered injuries that total more than -1x your HP; Good Luck.\nThe effects of injuries stated above still apply, such as HT rolls to stay conscious.");
			ImGui::BulletText("Once you are injured to -1x HP, make an HT roll. If you fail, you die. That's it. \nAt a degree of failure of -1 or -2 you may cheat death, but you are still dying.\nIf you continue to be injured while in this state, you must roll \nthe same HT roll, with the same consequences, for every multiple of \ndamage that you suffer from in this state.\n");
			ImGui::TreePop();
		}


		//This Information set is SUPER long, but if you want more neatly nested text, use this default text pile.
		/*
		ImGui::Text("\n");
		if (ImGui::TreeNode("NestedText:"))
		{
		ImGui::BulletText("Bullet:");
		ImGui::Text("Text");
		ImGui::TreePop();
		}
		*/
		ImGui::Text("\n");
		ImGui::TreePop();
	}
};



void damageRoll()
{
	//Arguably the most complicated of the bunch. When complete, this function handles rolling for damage,
	//Given the statistics of a character's weapon, and, when they take damage, how much their DR Absorbs or gets penetrated. 
	if (ImGui::CollapsingHeader("\nDamage Roll Handler\n\n"))
	{

		ImGui::Text("When your attack strikes, use these tools to determine the damage output. \nIf you take damage, this tool also determines how much damage to your HP \nthat you have taken when something has hit you, or the other way around. \n\n\nFor additional details See Page 369-374 of Gurps 4e Basic Set: Campaigns");
		//BEGINNING COMBAT INFORMATION
		damageRollAddInfo();

		//END OF COMBAT INFORMATION
		srand(time(NULL));

		//LOCAL VARIABLE DECLARATION
		static int skill = 10;
		static int dam_Modifier = 0;
		static int dam_roll = 0;
		static int degree = 0;
		static int pen_damage = 0;
		static int selected_DMG = -1;
		static int selected_Melee = -1;
		static int numDie = 1;
		static int multiplier = 1;
		//static unsigned int multiplier = 1;
		static int raw_damage = 0;
		static int raw_damage_eval = 0;
		static bool isMelee = false;
		static bool isDamHandIO = false;
		//DAMAGE INPUTS

		//MELEE WEAPON RULES
		ImGui::Text("\n\n");
		ImGui::Text("__________MELEE DAMAGE HANDLING_______________________\n");

		if (isDamHandIO == false)
		{
			ImGui::Text("If your Weapon Uses Muscle Power, Select the box below to quicky ready your roll.\nThis includes Thrown and Melee Weapons, Bows, and Crossbows.\n");
			ImGui::Checkbox("Muscle Powered Weapon Equipped?", &isMelee);

			ImGui::SameLine(); GURPS_ShowHelpMarker("Muscle Powered Weapons have damage based on a PC's Strength,\nDamage Type, and a plus or minus modifier.\nFor example, an Axe deals Swing+2 damage. To enter this, \nset Damage Type to Swing, enter the axe user's Strength,\nand set the Melee Modifier to 2.");

			if (isMelee == true)
			{

				static int pcStrength = 10;
				static int meleeMod = 0;
				const char* damMelee[] = { "Swing", "Thrust" };
				//Melee Modifier
				if (ImGui::Button("Select Melee Damage Type\n\n")) 
				{
					ImGui::OpenPopup("selectMelee");
				}
				ImGui::SameLine();
				ImGui::TextUnformatted(selected_Melee == -1 ? "<None Selected>" : damMelee[selected_Melee]);
				ImGui::SameLine(); GURPS_ShowHelpMarker("Your melee attack will be a Swing or a Thrust, \nchoose as shown on your weapon's table. \n");

				if (ImGui::BeginPopup("selectMelee"))
				{
					ImGui::Text("Damage:");
					ImGui::Separator();
					for (int i = 0; i < IM_ARRAYSIZE(damMelee); i++)
						if (ImGui::Selectable(damMelee[i]))
							selected_Melee = i;
					ImGui::EndPopup();
				}
				ImGui::SliderInt("PC Strength", &pcStrength, 1, 20);
				ImGui::SameLine(); GURPS_ShowHelpMarker("Your melee Damage, with tooth ,bowstring, or steel, is highly dependent on Strength. \nThe stronger you are, the harder you will hit with Swings and Thrusts.\nYou will also be able to draw more powerful bowstrings.");
				ImGui::SliderInt("Sw/Thr +(_) Modifier", &meleeMod, -20, 20);
				ImGui::SameLine(); GURPS_ShowHelpMarker("Enter the Melee Modifier Here. as with the Swing+2 example above, add the +2 here");



				if (ImGui::Button("\nApply Melee Damage Attributes:\n\n"))
				{
					dam_Modifier = (DamageTableMod(pcStrength, damMelee[selected_Melee]) + meleeMod);
					numDie = DamageTableDie(pcStrength, damMelee[selected_Melee]);
				}
				ImGui::SameLine(); GURPS_ShowHelpMarker("Press to override the values in the Basic Damage Handler\nwith the Weapon Data you have entered above.");
				//END OF MELEE DAMAGE HANDLING
			}
		}
		else
		{
			ImGui::Text("Melee Damage Input Collapsed.");
		}


			ImGui::Text("\n");
			ImGui::Text("__________BASIC DAMAGE INPUT_______________________\n");


			ImGui::Checkbox("Collapse Damage Roll Input?", &isDamHandIO);
			ImGui::SameLine(); GURPS_ShowHelpMarker("Un-check this box to collapse the Input Area.");
			if (isDamHandIO == false)
			{

				ImGui::Text("Enter Weapon Table Data");
				ImGui::SliderInt("(__)d Die", &numDie, 1, 8);
				ImGui::SameLine(); GURPS_ShowHelpMarker("Damage in GURPS is arranged in a Dice + Adds format.\nFor example, for a damage of 3d-2, slide (__)d to 3 dice and subtract 2 from the results using the Melee Modifier below it.");

				ImGui::SliderInt("Basic Modifier", &dam_Modifier, -8, 8);
				ImGui::SameLine(); GURPS_ShowHelpMarker("This would be the + or - Value adjacent to the number of die.\nSome Weapons are slightly better or worse at what they do than others, even among an identical weapon class. \nBe sure to include this Stat from your Weapon's table. \n");

				ImGui::Text("Die Multiplier/Multiple Shot Counter");
				ImGui::InputInt("d X (__)", &multiplier);
				//ImGui::SliderInt("()d Times__", &multiplier, 1, 300 );//Slider just won't work here.
				ImGui::SameLine(); GURPS_ShowHelpMarker("To prevent excessive die rolling, a roll multiplier is used from X2-3 to the thousands.\nFor Example, for 6dX5, roll six dice and multiply the total damage by 5.\nOr, in the event of a weapon that hits more than 1 time, use this to your advantage.");
				ImGui::Text("Note: This Input CAN be zero. Watch out.");



				//Tight beam Burning has a lot of different properties to account for, so it won't be included fro the time being.
				//WEAPON EFFECT HANDLING
				ImGui::Text("\nEnter the Wounding Modifier of the weapon you are using here.\nNote: Tight Beam Burning Damage(Ex:Laser) is not represented here. \nFor that Modifier, see Page 399, 433, and 434.");
				const char* damType[] = { "Small piercing(pi-)", "Piercing(pi)", "Large Piercing(pi+)", "Huge Piercing(pi++)", "Cutting(cut)" , "Impaling(imp)","Burning(burn)","Corrosion(cor)", "Crushing(cr)", "Fatigue(fat)", "Toxic(tox)" };
				if (ImGui::Button("Select Wounding Modifier of Selected Weapon:\n\n"))
					ImGui::OpenPopup("woundingSelector");
				ImGui::SameLine();
				ImGui::TextUnformatted(selected_DMG == -1 ? "<None>" : damType[selected_DMG]);
				if (ImGui::BeginPopup("woundingSelector"))
				{
					ImGui::Text("Wounding Modifer:");
					ImGui::Separator();
					for (int i = 0; i < IM_ARRAYSIZE(damType); i++)
						if (ImGui::Selectable(damType[i]))
							selected_DMG = i;
					ImGui::EndPopup();
				}
				ImGui::SameLine(); GURPS_ShowHelpMarker("A Wounding Modifier can greatly affect damage output.\nRefer to your weapon's Wounding Modifier in its statistics.");

				ImGui::Text("Round Fractions of Damage Down. Minimum Penetration Damage: 1HP.");

				//
				if (ImGui::Button("\n          Click to Roll for Basic Damage: \n\n"))
				{
					dam_roll = (rollMultipleDie(numDie));
				}
				ImGui::SameLine(); ImGui::Text("\nYou will roll: %dd+%d\n", numDie, dam_Modifier);


			}
			else
			{
				ImGui::Text("Basic Damage Roll Handler Collapsed.");

			}


			raw_damage = ((dam_roll + dam_Modifier) * multiplier);
			ImGui::Text("\n          |__________BASIC DAMAGE RESULTS__________|");
			ImGui::Separator();
			ImGui::Text("             Current BASIC DAMAGE Output:%d", raw_damage);
			//PENETRATIVE DAMAGE LOCAL VARIABLES
			static bool isDamHitIO = false;
			static bool useRoll = false;
			static bool is_DR_flexible = false;
			static bool is_DR_split = false;
			static int victim_DR = 0;
			static int victim_DR_split = 0;
			static int flex_DR = 0;
			static int absorbed_damage = 0;
			static int eval_selected_DMG = -1;
			//
			ImGui::Text("\n\n");
			ImGui::Text("__________PENETRATIVE DAMAGE HANDLING_______________________\n");
			ImGui::Checkbox("Collapse Pentrative Damage Input?", &isDamHitIO);
			if (isDamHitIO == false)
			{


				ImGui::Text("\n\n");
				ImGui::Checkbox("Use Basic Damage AND Wound Modifier from Current Basic Damage Roll", &useRoll);
				ImGui::SameLine(); GURPS_ShowHelpMarker("Check this box to use the Basic Damage and weapon properties you Evaluated Above.");
				if (useRoll == true)
				{
					ImGui::Text("You are using your own Basic Damage Roll to measure penetrated Damage.\n\n");

					raw_damage_eval = raw_damage;
				}
				else
				{
					ImGui::Text("Enter the amount of Basic Damage you, or an enemy, has dealt to measure penetrated damage.\nDO NOT forget to include the Wounding Modifier of the selected weapon Below.");
					ImGui::InputInt("Basic Damage", &raw_damage_eval);
					ImGui::SameLine(); GURPS_ShowHelpMarker("This is the Direct amount of damage Dealt by you or an opponent's attack.\n(Example: The DM says you took, say, 16 Damage.)\nIf you want to use the roll from earlier, check the box above.");

					//USER SHOULD INPUT THEIR OWN TYPE OF WOUNDING MODIFIER
					const char* eval_damType[] = { "Small piercing(pi-)", "Piercing(pi)", "Large Piercing(pi+)", "Huge Piercing(pi++)", "Cutting(cut)" , "Impaling(imp)","Burning(burn)", "Corrosion(cor)", "Crushing(cr)", "Fatigue(fat)", "Toxic(tox)" };
					if (ImGui::Button("Wounding Modifier of The Damage Dealt:"))
						ImGui::OpenPopup("Eval_select");
					ImGui::SameLine();
					ImGui::TextUnformatted(eval_selected_DMG == -1 ? "<None>" : eval_damType[eval_selected_DMG]);
					if (ImGui::BeginPopup("Eval_select"))
					{
						ImGui::Text("Selected Wounding Modifer:");
						ImGui::Separator();
						for (int i = 0; i < IM_ARRAYSIZE(eval_damType); i++)
							if (ImGui::Selectable(eval_damType[i]))
								eval_selected_DMG = i;
						ImGui::EndPopup();
					}
				}
				//THESE CALCULATIONS NEED TO BE PORTED INTO HELPER FUNCTIONS IN THE FUTURE.


				ImGui::InputInt("Victim's DR", &victim_DR);
				ImGui::SameLine(); GURPS_ShowHelpMarker("DR is technically you or your victim's armor.\n Usually, it directly subtracts any Basic Damage taken upon being hit. However,\n some types of attacks on armor will damage you without penetration. ");
				ImGui::Checkbox("Is the Victim's DR Split?", &is_DR_split);
				ImGui::SameLine(); GURPS_ShowHelpMarker("Some Types of Armor have a Split DR, where it is stronger/weaker against certain attacks.\nThe LEFT DR is the armor's DR against all cutting or piercing.\nThe RIGHT DR is against ALL other Damage Types.");
				if (is_DR_split == true)
				{
					ImGui::Text("Assuming the first DR entry is the LEFT side DR statistic, \nEnter the RIGHT side statistic of the armor here.");
					ImGui::InputInt("Victim's Split DR", &victim_DR_split);
					ImGui::SameLine(); GURPS_ShowHelpMarker("For example, a Ultra Tech ballistic(Reflex) vest \nhas a DR  across the Torso at 12/4. You have entered the DR of 12 earlier, now, you will enter the DR of 4 here.");
					//NEED TO FINISH
					ImGui::Text("\n");
				}
				ImGui::SameLine(); ImGui::Checkbox("Is the Victim's Armor Flexible?", &is_DR_flexible);
				ImGui::SameLine(); GURPS_ShowHelpMarker("Flexible Armor can cause blunt trauma, if all damage is absorbed.\nIf the armor is layered, any hit that penetrates\n all the way to the flexible armor, and is stopped by it,\n this blunt trauma is inflicted.");
				if (is_DR_flexible == true)
				{
					ImGui::Text("Flexible Armor will now be accounted for.");
					//NEED TO FINISH
				}

				//ImGui::SameLine(); GURPS_ShowHelpMarker("");
				//ImGui::Text("");
				if (ImGui::Button("Apply Penetrated Damage:"))
				{
					//if () 
					//{

					//}

					int temp_victim_DR = 0;
					int temp_damage = 0;
					int temp_damage_type = 0;
					//Call local variables to avoid modifying more useful variables.
					if (useRoll == true)
					{
						temp_damage = raw_damage;
						temp_damage_type = selected_DMG;
					}
					else
					{
						//User didnt' select the Use Roll, and we use the entered Damage.
						temp_damage = raw_damage_eval;
						temp_damage_type = eval_selected_DMG;
					}

					if (is_DR_split == true)
					{

						if ((temp_damage_type >= 0 && temp_damage_type <= 3) || temp_damage_type == 4)
						{

							temp_victim_DR = victim_DR_split;
						}
						else
						{
							temp_victim_DR = victim_DR;
						}

					}
					else
					{
						temp_victim_DR = victim_DR;
					}

					//SPLIT DR AND FLEXIBLE HANDLING COMPLETEHANDLING

					if (temp_damage > temp_victim_DR)
					{
						//Some Degree of Damage has penetrated!
						pen_damage = (temp_damage - temp_victim_DR);
						//These rules only affect Penetrated DR!
						if (temp_damage_type == 0)
						{
							//Small Piercing
							pen_damage = pen_damage * 0.5;
						}
						else if (temp_damage_type == 2 || temp_damage_type == 4)
						{
							//Cutting and Large Piercing
							pen_damage = pen_damage * 1.5;
						}
						if (temp_damage_type == 3 || temp_damage_type == 5)
						{
							pen_damage = pen_damage * 2;
						}
						//dam_roll = (rollMultipleDie(numDie));
					}
					else
					{
						//The Attack Bounced off, or didn't penetrate. Still, There's situations that may still cause damage.
						absorbed_damage = temp_damage;
						//pen_damage = (temp_damage / 10);
						if (is_DR_flexible == true)
						{
							if ((temp_damage_type >= 0 && temp_damage_type <= 4))
							{
								//Damage was penetrating 
								pen_damage = (absorbed_damage / 10);

							}
							else if (temp_damage_type == 8)
							{
								//Damage was Crushing
								pen_damage = (absorbed_damage / 5);
							}
						}


					}



				}

			}
			else
			{
				ImGui::Text("Basic Damage Roll Handler Collapsed.");

			}

			ImGui::Text("\n");
			ImGui::Text("          |__________FINAL RESULTS OF DAMAGE HANDLER__________|");
			ImGui::Separator();
			ImGui::Text("            Current HP DAMAGE PREVENTED:%d", absorbed_damage);
			ImGui::Text("\n          Current HP DAMAGE PENETRATED TO VICTIM:%d", pen_damage);
			/*
			//This Wounding Multiplier set is only for PENETRATED Damage, but it is stuffed here.
			//Since there's no place for resolving DR just yet.
			if (selected_DMG == 0)
			{
			pen_damage = pen_damage * 0.5;
			}
			else if (selected_DMG == 2 || selected_DMG == 4)
			{
			pen_damage = pen_damage * 1.5;
			}
			if (selected_DMG == 5 || selected_DMG == 6)
			{
			pen_damage = pen_damage * 0.5;
			}
			*/

			//ImGui::Text("Current Weapon Statistics: %d", dam_roll);


			//Description of what's next in this portion of Damage Roll Handler:
			//We need to create  a tool that should handle DR situations for both the PC delivering damage,
			//Or, the PC being damaged. Either we split this into two separate functions or we just resolve 
			//the problem with a check box like the melee damage above.


			//END OF DAMAGE ROLL HANDLER
			ImGui::Text("\n\n");
		}

		//END OF FUCNTION
	
};






void speedrangeTblAddInfo()
{

	ImGui::Text("\n          |__________ADDITIONAL INFORMATION__________|\n");
	if (ImGui::TreeNode("For more information regarding the Speed/Range Table, Select here."))
	{
		if (ImGui::TreeNode("What is the Speed/Range Table?"))
		{
			ImGui::Text("In Ranged Combat, A target's Size, Speed, and Range can modify your chances to hit.\n\nFor example, a broadside of a barn is easier to hit with a projectile weapon,\nbut, knocking the hat off of a train conductor as it speeds by is much harder.\nThis table ensures you can evaluate your chance to hit modifiers efficiently.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		//ImGui::TreePop();
		if (ImGui::TreeNode("Modifiers to the Speed/Range Table."))
		{
			//ImGui::Text("");
			ImGui::Text("The Speed/Range Table's main measurements are in SM(Size modifier), Yards, and Yards per Second.\n\nSM 0 is human sized target. Smaller targets have a lower SM, and larger targets have higher SM.\nTo find SM, measure the longest part of an object in Yards(Ex: head to tail of a beast).\nThis Linear Measurement is compared on the Speed/Range Table, rounded up, to determine its SM.\nAdd +2 to SM for objects that are uniform in shape. +1 for long, uniformly shaped objects.\n\nA target's Speed and Range can provide serious penalties to hit.\nA fighter jet may be easy to hit standing still on the runway, but it's much harder\nto hit the same jet taking a supersonic flyby a few yards above you.");
			ImGui::TreePop();
		}
		ImGui::Text("\n\n");
		

		ImGui::TreePop();
	}

};



void speedrangeTbl()
{
	//VARIABLE DECLARATIONS:

	float speed = 0;
	float range = 0;
	float size = 0;
	int calcModifier = 0;
	//Leaving original version of BigBadTable for now.
	float table[45][3] = 
	{
		{0,-15,(((1 / 5) / 12)) / 3},{ 0,-14,(((1 / 3) / 12) / 3) },{ 0,-13,(((1 / 2) / 12) / 3) },{ 0,-12,(((2 / 3) / 12) / 3) },{ 0,-11, (((1 / 12)) / 3) },{ 0,-10, (((3 / 2) / 12) / 3) },{ 0,-9, ((2 / 12) / 3) },{ 0,-8, ((3 / 12) / 3) },{ 0,-7, ((5 / 12) / 3) },{ 0,-6, ((8 / 12) / 3) },{ 0,-5, (1 / 3) },{ 0,-4, (1 / 3) },{ 0,-3, (2 / 3) },{ 0,-2, 1 },{ 0,-1, (3 / 2) },{ 0,0, 2 },{ -1,1, 3 },{ -2, 2, 5 },{ -3, 3, 7 },{ -4, 4, 10 },{ -5, 5, 15 },{-6, 6, 20},{-7,7,30},{-8,8,50},{-9,9,70},{-10,10,100},{-11,11,150},{-12,12,200},{-13,13,300},{-14,14,500},{-15,15,700},{-16,16,1000},{-17,17,1500},{-18,18,2000},{-19,19,3000},{-20,20,5000},{-21,21,7000},{-22,22,10000},{-23,23,15000},{-24,24,20000},{-25,25,30000},{-26,26,50000},{-27,27,70000},{-28,28,100000},{-29,29,150000}
	};
	//Use this version of BigBadTable for now.
	static float bigBadTable[46][3] =
	{
		{ 0,-15,0.0056 },{ 0,-14,0.00925 },{ 0,-13, 0.01389 },{ 0,-12,0.0185 },{ 0,-11, 0.0277 },{ 0,-10, 0.04167 },{ 0,-9, 0.056 },{ 0,-8, 0.083 },{ 0,-7, 0.1389 },{ 0,-6, 0.222 },{ 0,-5, 0.333 },{ 0,-4, 0.5 },{ 0,-3, 0.667 },{ 0,-2, 1 },{ 0,-1, (3 / 2) },{ 0,0, 2 },{ -1,1, 3 },{ -2, 2, 5 },{ -3, 3, 7 },{ -4, 4, 10 },{ -5, 5, 15 },{ -6, 6, 20 },{ -7,7,30 },{ -8,8,50 },{ -9,9,70 },{ -10,10,100 },{ -11,11,150 },{ -12,12,200 },{ -13,13,300 },{ -14,14,500 },{ -15,15,700 },{ -16,16,1000 },{ -17,17,1500 },{ -18,18,2000 },{ -19,19,3000 },{ -20,20,5000 },{ -21,21,7000 },{ -22,22,10000 },{ -23,23,15000 },{ -24,24,20000 },{ -25,25,30000 },{ -26,26,50000 },{ -27,27,70000 },{ -28,28,100000 },{ -29,29,150000 },{ -30,30,200000 }
	};
	//The tables in here are going to be a little packed in.

	//
	//
	//
	//_________U I INTERFACE BEGINS HERE:__________________
	//
	if (ImGui::CollapsingHeader("\nSpeed/Range Table Tool\n\n"))
	{
		ImGui::Text("Confused on how to use the ubiquitous Speed/Range Table? Calculate the results easily here!");
		speedrangeTblAddInfo();
		ImGui::Text("\n\n");
		ImGui::Text("__________SPEED AND RANGE TABLE HANDLING_______________________\n");

		ImGui::InputFloat("Target Speed", &speed);
		//ImGui::SameLine(); ImGui::Text("%f",table[][speed]);
		ImGui::SameLine(); ImGui::Text("Speed:%f",speed);
		ImGui::Text("\n\n");

	}


};



void combatStatusToolAddInfo()
{
	ImGui::Text("\n          |__________ADDITIONAL INFORMATION__________|\n");
	if (ImGui::TreeNode("For more information regarding Combat, Select here."))
	{
		if (ImGui::TreeNode("FirstHeader"))
		{
			ImGui::Text("\n");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		//ImGui::TreePop();
		//
		//These function demonstrations from the Demo should do nicely for future use.

		// General BeginCombo() API, you have full control over your selection data and display type.
		// (your selection data could be an index, a pointer to the object, an id for the object, a flag stored in the object itself, etc.)
		//const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
		//static const char* item_current = items[0];            // Here our selection is a single pointer stored outside the object.
		//if (ImGui::BeginCombo("combo 1", item_current, flags)) // The second parameter is the label previewed before opening the combo.
		//{
		//	for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		//	{
		//		bool is_selected = (item_current == items[n]);
		//		if (ImGui::Selectable(items[n], is_selected))
		//			item_current = items[n];
		//		if (is_selected)
		//			ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		//	}
		//	ImGui::EndCombo();
		//}

		// Simplified one-liner Combo() API, using values packed in a single constant string
		//static int item_current_2 = 0;
		//ImGui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

		// Simplified one-liner Combo() using an array of const char*
		//static int item_current_3 = -1; // If the selection isn't within 0..count, Combo won't display a preview
		//ImGui::Combo("combo 3 (array)", &item_current_3, items, IM_ARRAYSIZE(items));
		//This function set has 3 inline floats in it.
		//static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
		//ImGui::InputFloat3("input float3", vec4a);
		//

		//This function might do well for a notes page.
		//static char str0[128] = "Hello, world!"
		//ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));


		// Another very useful Demo of an alternate way to input text.
		//static char text[1024 * 16] =
		//"/*\n"
			//"\tlock cmpxchg8b eax\n";


		if (ImGui::TreeNode("Header"))
		{
			//ImGui::Text("");
			ImGui::Text("Text");
			ImGui::TreePop();
		}
		ImGui::Text("\n");

		ImGui::TreePop();
	}
};


void combatStatusTool()
{

	if (ImGui::CollapsingHeader("\nCombat Status Tool\n\n"))
	{



		ImGui::Text("To assist in combat, use this toolset to keep tactical information at hand. \nSelect Various status affects and maneuvers to Remain on top of your game. \n\n\nFor additional details See Page FIND APPROPRIATE PAGES of GURPS 4e Basic Set: Campaigns");

		combatStatusToolAddInfo();

		ImGui::Text("THIS SECTION IS UNFINISHED");

		//Some Features that would be welcome within this Tab include:
		//A Selectable Tab that has ALL Maneuvers within combat selectable, much like the melee thing.
		//When the user selects a Maneuver, Additional, verbose information about the maneuver should be displayed below it.
		//This should also include a Sub-Tab for Postures, if Possible, and their respective modifiers. 

		//Optional:A Range-to-size Table that a user can operate to get a quick reading of the respective modifiers at that specific range.
		//THIS IS NOW A SEPERATE TOOL.

		//A Tool Designed for Evaluating a PC/NPC's Defensive Capabilities.

		//A Chace to Hit Selectable item for differing common portions of the body. 

		//Up above in the Nested Additional information, Basic Information about Hexes and Ranging. 

		//Optional but would greatly add Value to the function: 
		//A sub-tool for damage handling, where you can keep track of your current health, as well as give feedback on 
		//your health. And, if your health reaches zero, shows handling for near death situations. 
		//In a panicked situation, a player could really use these tools!

		//ImGui::Text("Current 3d Roll: %d", roll);
		ImGui::Text("\n\n");
	}


};



void frightCheckAddInfo()
{
	ImGui::Text("          |__________ADDITIONAL INFORMATION__________|\n");
	if (ImGui::TreeNode("For more information regarding the Speed/Range Table, Select here."))
	{
		
		ImGui::Text("\n");

		ImGui::TreePop();
	}
}

void frightCheck()
{
	if (ImGui::CollapsingHeader("\nFright Check Handler\n\n"))
	{
		ImGui::Text("When incomprehensible horrors or unyielding divinity overwhelms the party,\nrefer to the Fright Check Table to determine their fate!");

		frightCheckAddInfo();


		ImGui::Text("Place the tools in the area beneath the Additional Information Tab.\n");

		ImGui::Text("\n\n");
	}

}


//Here's a Generalized Tool Structure, to make adding additional tools much easier to create.
void defaultToolAddInfo()
{
	ImGui::Text("          |__________ADDITIONAL INFORMATION__________|\n");
	if (ImGui::TreeNode("Most Tools have a section with additional info. Select here to expand it."))
	{
		if (ImGui::TreeNode("What is the Default Tool Structure?"))
		{
			ImGui::Text("These Tiered tabs will provide additional info on the above subject.\n");
			ImGui::TreePop();
		}
		ImGui::Text("\n");

		if (ImGui::TreeNode("More Useful Information."))
		{
			ImGui::Text("Some of these tabs may be quite the read, but their info is essential.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");


		ImGui::Text("\n");
		ImGui::TreePop();
	}
}


void defaultTool()
{	
	//I am also using the Default Tool as a sort of test bed for the contents of arrays and functions, 
	//and how these can be more easily managed by the user.
	static char* final_1d_resultChr = "Default 1d";
	static char* final_2d_resultChr = "Default 2d";
	static char* final_3d_resultChr = "Default 3d";
	const char* input_Desc[] = { "1/5 Inch or 0.5cm","1/3 Inch or 7.62cm","1/2 Inch or 1.27cm","2/3 Inch or 1.7cm","1 Inch or 2.54cm", "1.5 Inches or 3.8cm","2 Inches or 5.08cm","3 Inches or 7.62cm", "5 Inches or 12.7cm", "8 Inches or 20.32cm", "1 Foot or 0.3m", "1.5 Feet or 0.45m", "2 Feet or 0.6m","1 Yard or Appox 1m\n1yd = 1m in GURPS.","1.5 Yards or Meters", "2 Yards or Meters", "3 Yards or Meters", "5 Yards or Meters", "7 Yards or Meters", "10 Yards or Meters", "15 Yards or Meters", "20 Yards or Meters", "30 Yards or Meters", "50 Yards or Meters" , "70 Yards or Meters", "100 Yards or Meters" , "150 Yards or Meters", "200 Yards or Meters" , "300 Yards or Meters", "500 Yards or Meters" , "700 Yards or Meters", "1000 Yards or Meters" , "1500 Yards or Meters", "2000 Yards or Meters" , "3000 Yards or Meters", "5000 Yards or Meters" , "7000 Yards or Meters", "10000 Yards or Meters", "15000 Yards or Meters", "20000 Yards or Meters", "30000 Yards or Meters", "50000 Yards or Meters", "70000 Yards or Meters", "100000 Yards or Meters", "150000 Yards or Meters", "200000 Meters or Yards" };
	static char* OneD_Chr[] = { "A", "B","C","D" };
	static char* TwoD_Chr[] = {""};
	static char* ThrD_Chr[] = {""};

	static int final_integerValue = 0;

	static int final_sizeMod = 0;
	static int final_SMpenalty = 0;

	static float input_SMpenalty = 0;
	static char* bigBacess = "DEFAULT";
	static int input_sizeMod = 0;
	//static int baccessSelection = -1;
	static int baccessSelection = 15;
	//static float input ;

	static char* input_2d;
	//I'm mainly attempting to know more about the 2d arrays As far as I know, SpeedRange Table is accurate..
	//Big Bad Table should be a direct translation of the Speed Range Table from the reference Section of GURPS: Campaigns.
	static float bigBadTable[46][3] =
	{
		{ 0,-15,0.0056 },{ 0,-14,0.00925 },{ 0,-13, 0.01389 },{ 0,-12,0.0185 },{ 0,-11, 0.0277 },{ 0,-10, 0.04167 },{ 0,-9, 0.056 },{ 0,-8, 0.083 },{ 0,-7, 0.1389},{ 0,-6, 0.222 },{ 0,-5, 0.333 },{ 0,-4, 0.5 },{ 0,-3, 0.667 },{ 0,-2, 1 },{ 0,-1, (3 / 2) },{ 0,0, 2 },{ -1,1, 3 },{ -2, 2, 5 },{ -3, 3, 7 },{ -4, 4, 10 },{ -5, 5, 15 },{ -6, 6, 20 },{ -7,7,30 },{ -8,8,50 },{ -9,9,70 },{ -10,10,100 },{ -11,11,150 },{ -12,12,200 },{ -13,13,300 },{ -14,14,500 },{ -15,15,700 },{ -16,16,1000 },{ -17,17,1500 },{ -18,18,2000 },{ -19,19,3000 },{ -20,20,5000 },{ -21,21,7000 },{ -22,22,10000 },{ -23,23,15000 },{ -24,24,20000 },{ -25,25,30000 },{ -26,26,50000 },{ -27,27,70000 },{ -28,28,100000 },{ -29,29,150000 }, {-30,30,200000}
	};
	static float selected_firstDimTbl = 0;
	//static float bigBadArray = bigBadTable[2];
	static bool isDefaultExpand = false;

	//TUTORIAL RELATED VALUES:
	static int selectedArray = -1;
	static int selectedSlider = 0;
	static bool isSelectedBool = false;
	static int inputButton = 0;
	

	if (ImGui::CollapsingHeader("Default Tool Structure: \nClick HERE if you are a first time user!\n"))
	{
		ImGui::Text("\nThis is the general format of each Tool within the GURPSapp Game Aid.\n Click and drag the the lower-right corner of the app to resize it.\n\n");

		defaultToolAddInfo();

		ImGui::Text("\n\n");
		ImGui::Text("___________DEFAULT TOOL INPUT_______________________\n");
		ImGui::Text("\n");

		ImGui::Checkbox("Collapse Default Roll Input?", &isDefaultExpand);
		ImGui::SameLine(); GURPS_ShowHelpMarker("Select to shrink the Input Area.\nCollapse if you feel the workspace is too cluttered.");
		if (isDefaultExpand == false)
		{
			ImGui::Text("\nThe middle portion of each tool has a variety of inputs and (Info) markers,\nwhich are descriptive messages which better define a tool's input.");
			ImGui::Text("Hover over the (Info) marker to reveal these further details.\n");
			ImGui::SameLine(); GURPS_ShowHelpMarker("This is an (Info) Marker. It will give a description \nor additional information about a certain input.");

			ImGui::Text("\nIf you would like, review the variety of input methods below. \nMore input methods will be displayed for your perusal as you select them.");
			

			//INPUT TUTORIAL THING.

			ImGui::Text("\nSlide the Slider Input to 7 to see the next input example..");
			ImGui::SliderInt("Slider Input", &selectedSlider,-10,10); //ImGui::SameLine(50);
			ImGui::SameLine(); GURPS_ShowHelpMarker("You can click on an area of the slider to instantly select it!\n");
			ImGui::Text("Above is a Generic slider Input. Click and drag your cursor\non the Slider itself to select what value you wish to set it to.\n\n");

			if (selectedSlider == 7) 
			{
				ImGui::Text("\n\nCheck the 'Go Ahead and Check Me' box to see the next example.");
				ImGui::Checkbox("Go Ahead and Check Me", &isSelectedBool);
				ImGui::SameLine(); GURPS_ShowHelpMarker("You can click the text next to the checkbox to activate it as well!");
				ImGui::Text("This is a Checkbox. Not as much explaining to do on this one.");

				if (isSelectedBool == true) 
				{
					ImGui::Text("\n\nSet the Button Input below to 3 to see the next example.");
					ImGui::InputInt("A Button Input", &inputButton);
					ImGui::SameLine(); GURPS_ShowHelpMarker("Click the buttons or the value in the the Input area to modify the value.");
					ImGui::Text("Above is a Button Input. \nYou can Click the [-] [+] buttons to increase it by one.\nYou can also click on the value inside and enter it manually.");
					if (inputButton == 3) 
					{

						ImGui::Text("\n\nSelect the option labelled 'Select Me Please!' below.\nYou're almost done!");
						const char* selectTutorial[] = { "Don't Select Me!","Something else.","Select me please!","Another thing that you don't want.","Some other thing." };
						if (ImGui::Button("Select Something from this array  \nBy clicking on this button:\n\n"))
							ImGui::OpenPopup("selectTutorial");
						ImGui::SameLine();
						ImGui::TextUnformatted(selectedArray == -1 ? "Nothing Selected" : selectTutorial[selectedArray]);
						if (ImGui::BeginPopup("selectTutorial"))
						{
							ImGui::Text("Selecting Options From an Array:");
							ImGui::Separator();
							for (int i = 0; i < IM_ARRAYSIZE(selectTutorial); i++)
								if (ImGui::Selectable(selectTutorial[i]))
									selectedArray = i;
							ImGui::EndPopup();
						}
						ImGui::Text("Above is a Selectable Array. Clicking the button will open a drop down menu, \nallowing you to select one of many options available that you would want.\n\n");
						ImGui::SameLine(); GURPS_ShowHelpMarker("If a selection area is large enough, you may have to scroll through it.\n");
						if (selectedArray == 2)
						{
							ImGui::Text("Congrats! You have selected the 'Select me please!' option in the Array. \nYou are now familiarized with all input methods on this application.");
						}
					}

				}

			}



		}
		else
		{
			ImGui::Text("Default Tool Input is Collapsed.\n");
		}


		//This is some testing I am doing with sliders and multi-dimensional arrays.
		//
		//
		/*
		ImGui::TextUnformatted(selected_firstDimTbl == -1 ? "<None Selected>" : bigBadTable[selected_Melee]);
		ImGui::SameLine(); GURPS_ShowHelpMarker("Your melee attack will be a Swing or a Thrust, \nchoose as shown on your weapon's table. \n");

		if (ImGui::BeginPopup("selectMelee"))
		{
		ImGui::Text("Damage:");
		ImGui::Separator();
		for (int i = 0; i < IM_ARRAYSIZE(bigBadTable); i++)
		if (ImGui::Selectable(bigBadTable[0]))
		selected_Melee = i;
		ImGui::EndPopup();
		}
		*/
		//
		//ImGui::Text("Testing Features here. Ignore the slider and outputs if you wish.\n");


		//ImGui::Combo("BigArray Access", &baccessSelection, input_Desc, IM_ARRAYSIZE(input_Desc));
		//ImGui::SliderInt("Input Size Mod (SM)", &input_sizeMod,-15,30); //ImGui::SameLine(50);
		//ImGui::SliderFloat("Input Measurement", &input_SMpenalty, (((1 / 5) / 12)), 200000);



		//ImGui::SliderInt("1d Array Minupation", &OneD_Chr); //ImGui::SameLine(50);
		//ImGui::SameLine(); GURPS_ShowHelpMarker("Slide in the highest skill or attribute to roll against. \nIf you are rolling against an Attribute(HT,IQ,ST) more that 20, treat it as 20.");
		//ImGui::SliderInt("Modifier(Success)", &modifier0, -10, 10);
		//ImGui::SameLine(); GURPS_ShowHelpMarker("In certain situations, your chance of success\ncan be more or less favorable. \n\nThis modifier fine tunes your effective skill,\nwhich will modify your final rolled value.\n(Ex: -5 to your Running skill as your\nfoe has greased your escape path) \n\nAll Equipment and situational\nmodifiers can be tallied up here.");
		//
		//

		ImGui::Text("\n\n          |__________TOOL STRUCTURE RESULTS__________|");
		ImGui::Separator();
		ImGui::Text("\n          Returned outputs will appear near the bottom of the Tool.\n          Every returned result has a separating line defined beneath it.\n          They are indented to distance them from other onscreen data.");


		//The Textual results of my testing appear here. comment out in final build.
		//This shall be continued to be used during testing.

		//ImGui::Text("          Current Size Modifer: %d", input_sizeMod);
		//ImGui::Text("          Linear Measurement from BigBadTable: %f\n", bigBadTable[input_sizeMod+15][2]);
		//ImGui::Text("          BacessSelection Value: %d", input_sizeMod);


		//ImGui::Text("          Current 1d Result: %c", final_1d_resultChr);
		//ImGui::Text("          Current 2d Result: %c", final_2d_resultChr);
		//ImGui::Text("          Current 3d Result: %c", final_3d_resultChr);
		//ImGui::Text("          Linear Measurement from BigBadTable: %f\n", bigBadTable[input_SMpenalty][0]);





		//ImGui::Text("          SM from Bigarray: %d", baccessSelection - 15);


		//bigBadTable[input_sizeMod+15][2]

		ImGui::Text("\n\n");
	}

}







void glossTableAddInfo()
{

	ImGui::Text("          |__________TABLE OF CONTENTS__________|\n");
	if (ImGui::TreeNode("Select here to reveal the table of contents in the toolset."))
	{

		ImGui::Text("Success Roll Handler:\nUse for making a Generic Success Roll.\n");
		ImGui::Text("\n");
		ImGui::Text("Damage Roll Handler:\nInput or Roll for Damage, and see how much damage was dealt.\n");
		ImGui::Text("\n");
		ImGui::Text("Combat Status Tool:\nManage character states to aid in combat.\n");
		ImGui::Text("\n");
		ImGui::Text("Fright Check Handler:\nEvaluate just how a great and terrible thing affects you.\n");
		ImGui::Text("\n");
		ImGui::Text("Reaction Roll Handler:\nRoll to see if you've made a good impression on an NPC.\n");
		ImGui::Text("\n");
		ImGui::Text("Speed/Range Table Tool:\nCalculate your chance to hit something. Large or small.\n");
		ImGui::Text("\n");


		ImGui::TreePop();
	}
	ImGui::Text("\n");
	ImGui::Text("          |__________GLOSSARY OF GURPS TERMS__________|\n");
	if (ImGui::TreeNode("For more information regarding the Speed/Range Table, Select here."))
	{

		ImGui::Text("\n");

		if (ImGui::TreeNode("Character Related Terms"))
		{
			
			ImGui::Text("\n");
			ImGui::Text("Advantages:\n");
			ImGui::Text("\n");

			if (ImGui::TreeNode("Attributes:"))
			{

				ImGui::Text("\n");

				ImGui::TreePop();
			}

			ImGui::Text("\n");
			ImGui::Text("Disadvantage:\n");
			ImGui::Text("\n");
			ImGui::Text("Encumbrance:\n");
			ImGui::Text("\n");

			if (ImGui::TreeNode("Derived Characteristics:"))
			{

				ImGui::Text("\n");

				ImGui::TreePop();
			}
			ImGui::Text("\n");

			ImGui::TreePop();
		}

		ImGui::Text("\n");

		if (ImGui::TreeNode("Combat Related Terms"))
		{
			
			ImGui::Text("\n");
			ImGui::Text("Attack:\n");
			ImGui::Text("\n");
			ImGui::Text("Damage:\n");
			ImGui::Text("\n");
			ImGui::Text("Defense:\n");
			ImGui::Text("\n");
			ImGui::Text("Free Action:\n");
			ImGui::Text("\n");
			ImGui::Text("Injury:\n");
			ImGui::Text("\n");
			ImGui::Text("Maneuver:\n");
			ImGui::Text("\n");
			ImGui::Text("Speed/Range Table:\n");
			ImGui::Text("\n");
			ImGui::TreePop();
		}

		ImGui::Text("\n");

		if (ImGui::TreeNode("Equipment Related Terms"))
		{
			ImGui::Text("\n");
			ImGui::Text("Damage Resistance(DR):\n");
			ImGui::Text("\n");
			ImGui::Text("Defense Bonus(DB):\n");
			ImGui::Text("\n");
			ImGui::Text("Range:\n");
			ImGui::Text("\n");
			ImGui::Text("Tech Level (TL):\n");
			ImGui::Text("\n");

			ImGui::TreePop();
		}

		ImGui::Text("\n");

		if (ImGui::TreeNode("Roleplaying Related Terms"))
		{

			ImGui::Text("\n");
			ImGui::Text("Character:\n");
			ImGui::Text("\n");
			ImGui::Text("Game Master (GM):\n");
			ImGui::Text("\n");
			ImGui::Text("Non-Player Character (NPC):\n");
			ImGui::Text("\n");
			ImGui::Text("Party:\n");
			ImGui::Text("\n");
			ImGui::Text("Player Character (PC):\n");
			ImGui::Text("\n");
			ImGui::Text("Race:\n");
			ImGui::Text("\n");

			ImGui::TreePop();
		}
		
		ImGui::Text("\n");



		ImGui::TreePop();
	}

}

void glossTable()
{
	if (ImGui::CollapsingHeader("\nGlossary and Table of Contents\n\n"))
	{
		ImGui::Text("\n");

		glossTableAddInfo();

		ImGui::Text("\n\n");
	}

}


























