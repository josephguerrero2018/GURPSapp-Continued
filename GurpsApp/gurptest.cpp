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

	int d = (rand() % 6 + 1);

	return d;
}

int rollMultipleDie(int n)
{
	//Generic function for rolling any number of die, and return its values. 

	int d = 0;
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
	int degree = ((skill + modifier) - roll);
	if ((roll == 3 || roll == 4))
	{
		result = 4;
	}
	else if ((roll == 5 && skill >= 15) || (roll == 6 && skill >= 16))
	{
		result = 4;
	}

	//CRITICAL FAILIURE HANDLING
	else if (roll == 17)
	{
		if (skill <= 15)
		{
			result = 1;
		}
		else
		{
			result = 2;
		}
	}
	else if (roll == 18 || (degree <= -10))
	{
		result = 1;
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
	ImGui::Text("__________ADDITIONAL_INFORMATION_______________________\n");
	if (ImGui::TreeNode("For more information on other uses for Success rolls, Select here.\nSuccess Rolls are the Heart of GURPS. Be sure to familiarize yourself."))
	{
		if (ImGui::TreeNode("Contest Roll Information:"))
		{
			ImGui::Text("For a Quick Contest(ex:Two advenurers grabbing for the same shiny artifact);\nBoth contestants roll the same skill with the Success Roll Handler. The Highest roll wins. \n\nThe Degree of success breaks a tie, if that occurs.\nIf both parties have identical degrees of success, no one wins.");
			ImGui::Text("\nRegular Contests(ex:Two politicians at a debate);\nAre similar to Quick Contests, but if both parties succeed or fail, they re-roll. \n\nIf both contestants constantly have score of 6 or less, raise the lower score to 10, \nNext, add that amount to the higher roll and evaluate the winner.\nDo the opposite process if the two contestant's scores are higher than 14.\n");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		//ImGui::TreePop();
		if (ImGui::TreeNode("Resistance Roll Information:"))
		{
			//ImGui::Text("");
			ImGui::Text("In a Resistance Roll, an unwilling subject rolls to resist using a skill or attribute.\nThe attacker uses their attacking ability, and the defender, the related resistance.\n\nThis process follows the rules of a Quick Contest, but the attacker must win outright.\nIn the event of a tie, the defender resists the attack and is unharmed by the affliction.\n\nIf the attack is against a living, sapient subject, \nthe attacker's effective skill can't exceed 16, unless the defender's resistance is >16.\nOften, Resistance rolls are Will Rolls(By Default, your IQ), against fear, stress, and more.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		if (ImGui::TreeNode("Sense Roll Information:"))
		{
			ImGui::Text("Sense Rolls involve a roll against your senses to detect something, often hidden.\n(Ex:Tasting poison in the queen's wine at her coronation feast.)\nTo roll, use your Perception(By Default, your IQ) as the governing skill.\n\nItems or persons visally hidden, can be treated as a Quick Contest \nagainst the planter or hiding individual's soncealment skills (Holdout,Camouflage).\n\nFor situations of testing hearing, make a separate IQ Roll to make out speech.\nIn a Quick Contest role to stay silent, use the Stealth Skill.\nFor hearing distances, refer to Page 358 of the Basic Set: Campaigns.\n\nTaste and Smell are sensorily similar. The advantages Disciminitory Taste and Smell \ncan allow you to identify people, places, and things similar to standard vision.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		ImGui::TreePop();
	}
};



void successRoll()
{
	//This function handles Success Rolls and is called in the Main program.
	if (ImGui::CollapsingHeader("Success Roll Handler"))
	{
		srand(time(NULL));
		static int skill = 10;
		static int modifier0 = 0;
		static int roll = 10;
		static int degree = 0;
		int result = 3;
		ImGui::Text("Roll 3 Die against your skills, attributes, and situational modifiers. \nThis can be used for Attribute Rolls(ex:Will Rolls), Skill Rolls, \nand can help you determine if you succeed under pressure.\n\n\nFor additional details See Page 343-349 of Gurps 4e Basic Set: Campaigns");

		//ImGui::Text("For more information on other uses for Success rolls, Select here.");
		successRollAddInfo();
		//to preserve the clarity of the function, I'm moving the texty bits into seperate functions.
		
		ImGui::Text("__________SUCCESS_ROLL_INPUT_______________________\n");
		ImGui::SliderInt("Skill", &skill, 0, 20); ImGui::SameLine(150);
		ImGui::SameLine(); GURPS_ShowHelpMarker("Slide in the highest skill or attribute to roll against. \nIf you are rolling against an Attribute(HT,IQ,ST) more that 20, treat it as 20.");
		ImGui::SliderInt("Modifier(Success)", &modifier0, -10, 10);
		ImGui::SameLine(); GURPS_ShowHelpMarker("In certain situations, your success can be more or less favorable. \nThe modifier directly effects your effective skill, NOT your roll. \nAll combined Equipment and situational modifiers go here.");
		//If you want to change the values of the roll directly, enable the slider below for debug.
		ImGui::SliderInt("DEBUG: RollValue", &roll, 3, 18);
		
		if (ImGui::Button("Roll 3d."))
		    roll = rollMultipleDie(3);
			degree = ((skill + modifier0) - roll);
			result = success(skill, modifier0, roll);
		
		ImGui::Text("RESULTS");
		ImGui::Separator();
		ImGui::Text("Current 3d Roll: %d", roll);
		ImGui::Text("Final roll Value: %d", (roll+modifier0));
		ImGui::Text("Textual Results of Sucess Roll:");
		if (result == 1)
		{
			ImGui::Text("Disaster! You Critically Failed this roll.\nThe Degree of Success or Failiure was: %d", degree);
		}
		else if (result == 2)
		{
			ImGui::Text("Ouch! You Failed this roll.\nThe Degree of Success or Failiure was: %d", degree);
		}
		else if (result == 3)
		{
			ImGui::Text("Good! You Succeeded in this roll.\nThe Degree of Success or Failiure was: %d", degree);
		}
		else if (result == 4)
		{
			ImGui::Text("Great! You Critically Succeeded in this roll.\nThe Degree of Success or Failiure was: %d", degree);
		}
		ImGui::Text("\n\n");
	}
};





void reactionRolls()
{

	if (ImGui::CollapsingHeader("(GM)Reaction Rolls: "))
	{
		//Swiftly and clearly handles reaction rolls. 
		static int modifier2 = 0;
		static int reaction = 10;
		//default is 10, the average reaction.
		
		
		ImGui::Text("Roll 3 Die to determine the impressions of the NPC(s) to the PC(s). \n See Page 560 of Gurps 4e Basic Set: Campaigns");
		ImGui::SliderInt("Modifier(Rec)", &modifier2, -10, 10);
		ImGui::SameLine(); GURPS_ShowHelpMarker("To apply other biases of the NPC(s) involved,\nuse the slider to give advantage or disadvantage to the roll. \n most advantages or disadvantages are from 1 to 3.");
		ImGui::Text("__________REACTION_ROLL_INPUT_______________________\n");
		if (ImGui::Button("Roll 3d to Make Reaction Roll."))
			reaction = rollMultipleDie(3) + modifier2;
		
		ImGui::Text("Current Reaction Roll: %d", reaction);
		//ImGui::Text("Textual Results of Reaction Roll: %s", toolText);
		ImGui::Text("RESULTS");
		ImGui::Separator();
		ImGui::Text("Textual Results of Reaction Roll:");
		if( reaction <= 0)
		{
			ImGui::Text("The NPC hates the characters and will act in their worst interest. \n Nothing is out of the question, assault, betrayal, public ridicule, \n or ignoring a life-or-death plea are all possible.");
		}
		else if (reaction >= 1 && reaction <= 3) 
		{
			ImGui::Text("The NPC dislikes the characters and will act against them \nif it is convenient to do so. This can include attacking the party, \noffering grossly unfair terms in a transaction, and so on.");
		}
		else if (reaction >= 4 && reaction <= 6)
		{
			ImGui::Text("The NPC cares nothing for the characters and will act against them \nto misinform or endanger the party, if he/she can profit by doing so.");
		}
		else if (reaction >= 7 && reaction <= 9)
		{
			ImGui::Text("The NPC is unimpressed. He/she may make threats, demand a huge bribe \n before offering aid, or something similar.");
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
		ImGui::Text("\n\nRemember: Never Substitute these random Dice rolls for basic reason and logic.");

		ImGui::Text("\n\n");
	}
};


void damageRollAddInfo()
{
	ImGui::Text("__________ADDITIONAL_INFORMATION_______________________\n");
	if (ImGui::TreeNode("For more information regarding Combat, Select here.\n This information just might save your PC."))
	{
		if (ImGui::TreeNode("Basic Combat Flow and Turn sequence:"))
		{
			ImGui::Text("Combat in GURPS takes place each turn by a span a second or so. Despite that,\nCombat shouldn't always be in a second by second basis, and can include on the fly skill rolls!\nTurn order is detrmined by each PC/NPC's Basic Speed. PC tie's are broken by highest DX\nIf a tie is still in the way, it is broken by the GM randomly.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		//ImGui::TreePop();
		if (ImGui::TreeNode("Attack Flow During A Turn:"))
		{
			//ImGui::Text("");
			ImGui::Text("Here is the Basic Flow of an Attack within One Turn of Combat:");
			ImGui::BulletText("Make an attack roll(a Success Roll); rolling against your effective skills with your weapon.\nA Critical Success with an Attack Roll Skips a defender's Defense Roll.");
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
			ImGui::Text("During your turn, you can perform a variety of Maneuvers. Whatever Maneuver you choose in your turn \nwill determine what you can do on your turn, and open or close options for attack or defense.\nGeneral Descriptions of some Manuvers are below. for more details, see the Upcoming Combat Status Tool.");
			ImGui::BulletText("Do Nothing:\nTake any Active Defense, but No movement.");
			ImGui::BulletText("Attack:\nMake an armed attack, melee or ranged. Your weapons must be ready, and target in reach.");
			ImGui::BulletText("All Out Attack:\n Make an armed attack, with no effort to defend yourself.\nVariations of the All Out Attack are covered in the Combat Status Tool.");
			ImGui::BulletText("All Out Defense:\nIncrease your defense options at the cost of attack oppurtunity. \nDefensive moves vary, and are referenced in the Combat Status Tool.");
			ImGui::BulletText("Move:\nMove any number of hexes/yards, up to your maximum Move Distance, and take any Active Defense.\n");
			//If you run forward for 2+Turns, you receive a 20 percent Move Bonus rounded down.
			ImGui::BulletText("Change Posture:\nSwitch between one posture to another. You cannot move while changing posture.\nMore detailed info on these postures are in the Combat Status Tool");
			ImGui::BulletText("Ready:\nGrab or draw any Item and prepare it for use. Some weapons need a Ready Manuver to be re-used.\nReady manuvers can be used to operate physical items during combat. \nTake any active defense, but movement is limited to a Step.");
			ImGui::BulletText("Aim:\nWith specified weapon and target, spend the turn aiming at them. \nIf Aim is followed by an attack, add your weapon's accuracy to your effective skill. \nInjury while aiming needs a Will Roll to check if you lost your aim.\nTake any active defense at the cost of your aim. You can take a step as well.\nSimilar Manuvers like Evaluate, Concentrate, and Wait are in the Upcoming Combat Status Tool.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		if (ImGui::TreeNode("Free Actions:"))
		{
			ImGui::Text("Free Actions can be completed during any manuver. These include:");
			ImGui::BulletText("Speech:\nNo matter what, you can always talk in a manuver. A sentence or two per turn is reasonable.");
			ImGui::BulletText("Maintain Supernatural Ability:\nAs long as you are active, you can maintain a spell or psionic ability.");
			ImGui::BulletText("Drop a Ready item:\nYou can drop any item that you have readied. You can drop the item anywhere in reach.");
			ImGui::BulletText("Crouch:\n If possible, you can crouch (or rise from a crouch to stand) at the start of your turn.\nMovement will be slowed, and you can't sprint, but you will be a harder target to hit.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		if (ImGui::TreeNode("Non-Fatal Injury and You: When the Dog Bites/Bee Stings."))
		{
			ImGui::Text("Your HP is determined by your Strength, one to one.\nYou will take damage if your opponent's attack penetrates completely through your DR.\nThe Penetrated damage is then modified by the weapon's wounding modifier.\n\nAny penetrated attack is devastating to you or an NPC's combat effectiveness.\n");
			ImGui::BulletText("If you have less than 1/3 of your HP, you're badly wounded, Move and Dodge are halved.\n");
			ImGui::BulletText("If you have lost HP during a Turn, you are briefly in Shock for your next turn.\nYour DX and IQ for the next turn will be reduced in equal measure to HP loss, up to -4.\nShock doesn't penalize active defensive manuvers, but it will cripple other skils briefly.");
			ImGui::BulletText("If you sustain a single injury more than 1/2 of your HP in most situations,\n or a body part is injured to the point of crippling, you have a Major Wound.\nTo avoid knockdown and stun from such a grave injury, Take an HT Roll. \nThe HT roll is taken at HT-5 for face, Male groin, or vitals, (and the Male groin),\n HT-10 if wounded in the Skull or Eye, HT+3 and HT-4 for High and Low Pain Threshold.\n If the HT roll failed, you are stunned. Fall prone and drop anything you were carrying.\nWith a degree of failiure greater than 5, you fall unsconsious.");
			ImGui::BulletText("if you have 0 or less HP, you are in danger of Collapse.\nOn your next turn, make an HT roll at -1 of every full multiple of HP below 0.\nIf Successful, you proceed as usual, but you must continue succeeding to stay in battle.\nIf you Do Nothing, you only need to roll to do any other action to remain conscious.\nIf you fail the HT roll, you fall unconscious.");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		if (ImGui::TreeNode("Mortal Injury: Knocking on Heaven's Door."))
		{
			ImGui::Text("If you are here, you have suffered injuries that total more than -1x your HP; Good Luck.\nThe effects of injuries stated above still apply, like HT rolls to stay lucid.");
			ImGui::BulletText("Once you are injured to -1x HP, make an HT roll. If you fail, you die. \nAt a degree of failiure of -1 or -2 you may cheat death, but you are still dying.");
			ImGui::TreePop();
		}
		//We Could really use a Tooltip on Dying. Worded so that it applies to both PC's and NPCs. 
		//A good explanation of the process is on page 380 of the Gurps basic Set: Campaigns.

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
	if (ImGui::CollapsingHeader("Damage Roll Handler"))
	{
		
		ImGui::Text("When your attack strikes, use these tools to determine the damage output. \nIf you take damage, this tool also determine how much damage to your HT you have taken. \n\n\nFor additional details See Page 369-374 of Gurps 4e Basic Set: Campaigns");
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
		static int raw_damage = 0;
		static int raw_damage_eval = 0;
		static bool isMelee = false;
		//DAMAGE INPUTS
		
		//MELEE WEAPON RULES
		ImGui::Text("\n");
		ImGui::Text("__________MELEE_DAMAGE_HANDLING_______________________\n");
		ImGui::Text("If your Weapon Uses Muscle Power, Select the box below to use their rules.\nThis includes Thrown weapons, Bows and Crossbows.\n");
		ImGui::Checkbox("Muscle Powered Weapon Equipped.", &isMelee);
		ImGui::SameLine(); GURPS_ShowHelpMarker("Applying Thrust or Swing may not be all you need to input.\nMost Of these weapons have a further modifier.\nFor Example, an Axe is Swing+2.\nYou still need to move the Modifier slider by 2 increments.");

		if (isMelee == true)
		{
			
			static int pcStrength = 10;
			ImGui::SliderInt("PC Strength", &pcStrength, 1, 20);
			ImGui::SameLine(); GURPS_ShowHelpMarker("Your melee Damage, with tooth or sword, is highly dependent on Strength,");

			const char* damMelee[] = { "Swing", "Thrust" };
			if (ImGui::Button("Select Melee Damage Type"))
				ImGui::OpenPopup("selectMelee");
			ImGui::SameLine();
			ImGui::TextUnformatted(selected_Melee == -1 ? "<None Selected>" : damMelee[selected_Melee]);
			if (ImGui::BeginPopup("selectMelee"))
			{
				ImGui::Text("Damage:");
				ImGui::Separator();
				for (int i = 0; i < IM_ARRAYSIZE(damMelee); i++)
					if (ImGui::Selectable(damMelee[i]))
						selected_Melee = i;
				ImGui::EndPopup();
			}
			if (ImGui::Button("Apply Melee Damage Attributes:"))
			{
				dam_Modifier = DamageTableMod(pcStrength, damMelee[selected_Melee]);
				numDie = DamageTableDie(pcStrength, damMelee[selected_Melee]);
			}
			//END OF MELEE DAMAGE HANDLING
		}
		ImGui::Text("\n");
		ImGui::Text("__________BASIC_DAMAGE_INPUT_______________________\n");
		ImGui::Text("Enter Weapon Table Data");
		ImGui::SliderInt("(__)d Die", &numDie, 1, 8);
		ImGui::SameLine(); GURPS_ShowHelpMarker("Damage in GURPS is arranged in a Dice + Adds format.\nFor example, for a damage of 3d-2, slide (__)d to 3 dice and subtract 2 from the results using the modifier below it.");

		ImGui::SliderInt("modifier", &dam_Modifier, -8, 8);
		ImGui::SameLine(); GURPS_ShowHelpMarker("This would be the + or - Value adjacent to the number of die.\nSome Weapons are slightly better or worse at what they do than others, even among an identical weapon class. \nBe sure to inlcude this Stat from your Weapon's table. \n");
		
		ImGui::Text("Die Multiplier/Multiple Shot Counter");
		ImGui::InputInt("()d X __", &multiplier);
		//ImGui::SliderInt("()d Times__", &multiplier, 1, 300 );//Slider just won't work here.
		ImGui::SameLine(); GURPS_ShowHelpMarker("To prevent excessive die rolling, a roll multiplier is used from X2-3 to the thousands.\nFor Example, for 6dX5, roll six dice and multiply the total damage by 5.\nOr, in the event of a weapon that hits more than 1 time, use this to your adantage.");
		
		


		//WEAPON EFFECT HANDLING
		ImGui::Text("\nEnter the Wounding Modifer of the weapon you are using here.");
		const char* damType[] = { "Small piercing(pi-)", "Piercing(pi)", "Large Piercing(pi+)", "Huge Piercing(pi++)", "Cutting(cut)" , "Impaling(imp)","Burning(burn)", "Corrosion(cor)", "Crushing(cr)", "Fatigue(fat)", "Toxic(tox)"};
		if (ImGui::Button("Select Wounding Modifer of Selected Weapon:"))
			ImGui::OpenPopup("select");
		ImGui::SameLine();
		ImGui::TextUnformatted(selected_DMG == -1 ? "<None>" : damType[selected_DMG]);
		if (ImGui::BeginPopup("select"))
		{
			ImGui::Text("Wounding Modifer:");
			ImGui::Separator();
			for (int i = 0; i < IM_ARRAYSIZE(damType); i++)
				if (ImGui::Selectable(damType[i]))
					selected_DMG = i;
			ImGui::EndPopup();
		}
		ImGui::SameLine(); GURPS_ShowHelpMarker("A Wounding Modifier can greatly affect damage output.\nRefer to your weapon's Wounding Modifer in its statistics.");

		ImGui::Text("Round Fractions of Damage Down. Minimum Penatrative Damage: 1HP.");

		//
		if (ImGui::Button("Roll For Basic Damage:"))
		{
			dam_roll = (rollMultipleDie(numDie));
		}

		

		raw_damage = ((dam_roll + dam_Modifier) * multiplier);
		ImGui::Text("\nBASIC DAMAGE RESULTS");
		ImGui::Separator();
		ImGui::Text("Current BASIC DAMAGE Output:%d",raw_damage);
		//PENETRATIVE DAMAGE LOCAL VARIABLES
		static bool useRoll = false;
		static bool is_DR_flexible = false;
		static bool is_DR_split = false;
		static int victim_DR = 0;
		static int victim_DR_split = 0;
		static int flex_DR = 0;
		static int absorbed_damage = 0;
		static int eval_selected_DMG = -1;
		//
		ImGui::Text("\n");
		ImGui::Text("__________PENETRATIVE_DAMAGE_HANDLING_______________________\n");
		ImGui::Checkbox("Use Basic Damage AND Wound Modifier from Current Basic Damage Roll", &useRoll);
		ImGui::SameLine(); GURPS_ShowHelpMarker("Check this box to use the Basic Damage and weapon properties you Evaluated Above.");
		if (useRoll == true)
		{
			ImGui::Text("You are using your own Basic Damage Roll to measure penetrated Damage.");
			
			raw_damage_eval = raw_damage;
		}
		else 
		{
			ImGui::Text("You Must Enter the amount of Basic Damage you,\nor an enemy, has dealt to measure penetrated damage.\nDO NOT forget to include the Wounding Modifier Below.");
			ImGui::InputInt("Basic Damage", &raw_damage_eval);
			ImGui::SameLine(); GURPS_ShowHelpMarker("This is the Direct amount of damage Dealt by you or an opponent's attack.\n(Example: The DM says you took, say, 16 Damage.)\nIf you want to use the roll from earlier, check the box above.");
			
			//USER SHOULD INPUT THEIR OWN TYPE OF WOUNDING MODIFIER
			const char* eval_damType[] = { "Small piercing(pi-)", "Piercing(pi)", "Large Piercing(pi+)", "Huge Piercing(pi++)", "Cutting(cut)" , "Impaling(imp)","Burning(burn)", "Corrosion(cor)", "Crushing(cr)", "Fatigue(fat)", "Toxic(tox)" };
			if (ImGui::Button("Wounding Modifer of The Damage Dealt:"))
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
		ImGui::SameLine(); GURPS_ShowHelpMarker("Flexible Armor can cause blunt trauma, if all damage is absorbed.\nIf the armor is layered, any armor that penetrates\n all the way to the flexible armor can cause this trauma.");
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
				if (is_DR_flexible == true ) 
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
		ImGui::Text("\n");
		ImGui::Text("FINAL RESULTS");
		ImGui::Separator();
		ImGui::Text("Current HP DAMAGE PREVENTED:%d", absorbed_damage);
		ImGui::Text("\nCurrent HP DAMAGE PENETRATED TO VICTIM:%d", pen_damage);
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
		//Or, the PC being damaged. Either we split this into two seperate functions or we just resolve 
		//the problem with a check box like the melee damage above.


	//END OF DAMAGE ROLL HANDLER
		ImGui::Text("\n\n");
	}

	//END OF FUCNTION
};


void combatStatusToolAddInfo()
{
	ImGui::Text("__________ADDITIONAL_INFORMATION_______________________\n");
	if (ImGui::TreeNode("For more information regarding Combat, Select here."))
	{
		if (ImGui::TreeNode("FirstHeader"))
		{
			ImGui::Text("\n");
			ImGui::TreePop();
		}
		ImGui::Text("\n");
		//ImGui::TreePop();
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

	if (ImGui::CollapsingHeader("Combat Status Tool"))
	{
		
		

		ImGui::Text("To assist in combat, use this toolset to keep tactical information at hand. \nSelect Various status affects and manuvers to Remain on top of your game. \n\n\nFor additional details See Page FIND APPROPRIATE PAGES of Gurps 4e Basic Set: Campaigns");
		

		ImGui::Text("THIS SECTION IS UNFINISHED");

		//Some Features that would be welcome within this Tab include:
		//A Selectable Tab that has ALL Manuvers within combat selectable, much like the melee thing.
		//When the user selects a Manuver, Additional, verbose information about the manuver should be displayed below it.
		//This should also include a Sub-Tab for Postures, if Possible, and their respective modifiers. 

		//Optional:A Range-to-size Table that a user can operate to get a quick reading of the respective modifiers at that specific range.

		//A Tool Designed for Evaluating a PC/NPC's Defensive Capabilities.

		//A Chace to Hit Selectable item for differing common portions of the body. 

		//Up above in the Nested Additional information, Basic Infomration about Hexes and Ranging. 

		//Optional but would greatly add Value to the function: 
		//A sub-tool for damage handling, where you can keep track of your current health, as well as give feeback on 
		//your health. And, if your health reaches zero, shows handling for near death situations. 
		//In a panicked situation, a player could really use these tools!

		//ImGui::Text("Current 3d Roll: %d", roll);
		ImGui::Text("\n\n");
	}

	
};


