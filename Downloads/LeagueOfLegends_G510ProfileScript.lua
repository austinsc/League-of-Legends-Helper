startTime = 0
initialJungleStart = 1000 --115000

function GetElapsedTime()
	return GetRunningTime() - startTime
end

function WriteMessage(message)
	OutputLCDMessage(message, 1000)
	OutputLCDMessage(GetElapsedTime(), 1000)
	ClearLCD()
end

function BlinkMessage(message, blinks)
	for i = 0, blinks, 1 do
		OutputLCDMessage(message, 100)
		Sleep(20)
		OutputLCDMessage("   ", 100)
		ClearLCD()
	end
end

function InitializeGameVariables()
	OutputLCDMessage("Starting up...", 3000)
	startTime = GetRunningTime()
	while startTime > 0 do
	end
	OutputLCDMessage("Shutting down...", 3000)
end


function OnEvent(event, arg)
	--OutputLogMessage("event = %s, arg = %s\n", event, arg);
	-- Display some text for 2 seconds
	if event == "G_PRESSED" then 
		if arg == 1 then
			if IsModifierPressed("ctrl") then
				InitializeGameVariables()
			elseif IsModifierPressed("alt") then
				startTime = 0
			else
				WriteMessage("Friendly Blue Buff")
			end
		elseif arg == 2 then	
			WriteMessage("Friendly Red Buff")
		elseif arg == 4 then	
			WriteMessage("Enemy Blue Buff")
		elseif arg == 5 then	
			WriteMessage("Enemy Red Buff")
		elseif arg == 3 then	
			WriteMessage("Dragon")
		elseif arg == 6 then	
			WriteMessage("Baron")
		end
	end
end