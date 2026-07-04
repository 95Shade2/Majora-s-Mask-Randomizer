using System;
using System.IO;
using System.Text;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class BingoCardExport
    {
        public static string BuildPlainText(BingoCard card)
        {
            StringBuilder text = new StringBuilder();
            text.AppendLine("Majora's Mask Randomizer — Bingo Card");
            AppendMetadata(text, card);
            text.AppendLine();
            AppendGrid(text, card);
            text.AppendLine();
            AppendRules(text, card);
            if (card.RerollTrace != null && card.RerollTrace.Count > 0)
            {
                text.AppendLine();
                text.Append(BingoRerollLog.Format(card));
            }
            return text.ToString();
        }

        public static string BuildHtml(BingoCard card)
        {
            StringBuilder html = new StringBuilder();
            html.AppendLine("<!DOCTYPE html>");
            html.AppendLine("<html><head><meta charset=\"utf-8\"/>");
            html.AppendLine("<title>MMR Bingo Card</title>");
            html.AppendLine("<style>");
            html.AppendLine("body{font-family:Segoe UI,Arial,sans-serif;margin:24px;color:#222;}");
            html.AppendLine("table{border-collapse:collapse;margin:16px 0;}");
            html.AppendLine("td,th{border:1px solid #444;padding:10px;vertical-align:middle;text-align:center;width:140px;height:80px;}");
            html.AppendLine("th{background:#eee;}");
            html.AppendLine(".line-header{cursor:pointer;text-decoration:underline;}");
            html.AppendLine(".goal-cell{cursor:pointer;user-select:none;}");
            html.AppendLine(".meta{margin-bottom:12px;line-height:1.5;}");
            html.AppendLine(".mark-green{background:#c8e6c9;}");
            html.AppendLine(".mark-red{background:#ffcdd2;}");
            html.AppendLine(".mark-orange{background:#ffe0b2;}");
            html.AppendLine(".mark-blue{background:#bbdefb;}");
            html.AppendLine(".mark-purple{background:#e1bee7;}");
            html.AppendLine("#popout-overlay{display:none;position:fixed;inset:0;background:rgba(0,0,0,.35);z-index:1000;align-items:center;justify-content:center;}");
            html.AppendLine("#popout-overlay.open{display:flex;}");
            html.AppendLine("#popout-panel{background:#fff;border:1px solid #444;padding:12px;max-width:95vw;max-height:90vh;overflow:auto;}");
            html.AppendLine("#popout-title{font-weight:bold;margin-bottom:8px;}");
            html.AppendLine("#popout-grid{display:flex;gap:8px;}");
            html.AppendLine("#popout-grid.vertical{flex-direction:column;}");
            html.AppendLine("#popout-grid.horizontal{flex-direction:row;flex-wrap:wrap;}");
            html.AppendLine(".popout-cell{border:1px solid #444;padding:12px;min-width:120px;min-height:72px;cursor:pointer;user-select:none;text-align:center;}");
            html.AppendLine("</style></head><body>");
            html.AppendLine("<h1>Majora's Mask Randomizer — Bingo Card</h1>");
            html.AppendLine("<div class=\"meta\">");
            AppendMetadataHtml(html, card);
            html.AppendLine("</div>");
            html.AppendLine("<table id=\"bingo-grid\">");
            html.AppendLine("<tr><th class=\"line-header\" data-line=\"card\">CARD</th>");
            for (int col = 0; col < 5; col++)
            {
                html.AppendLine("<th class=\"line-header\" data-line=\"col\" data-index=\"" + col + "\">COL-" + (col + 1) + "</th>");
            }

            html.AppendLine("<th></th></tr>");

            for (int row = 0; row < 5; row++)
            {
                html.Append("<tr><th class=\"line-header\" data-line=\"row\" data-index=\"" + row + "\">ROW-" + (row + 1) + "</th>");
                for (int col = 0; col < 5; col++)
                {
                    int index = row * 5 + col;
                    int state = card.GoalStates != null && index < card.GoalStates.Length ? card.GoalStates[index] : 0;
                    string cssClass = BingoGoalMark.GetHtmlClass(state);
                    string classAttr = string.IsNullOrEmpty(cssClass) ? "goal-cell" : "goal-cell " + cssClass;
                    html.Append("<td class=\"" + classAttr + "\" data-index=\"" + index + "\" data-state=\"" + state + "\">");
                    html.Append(FormatGoalHtml(card.Goals[index]));
                    html.Append("</td>");
                }

                html.AppendLine("<th></th></tr>");
            }

            html.AppendLine("<tr><th class=\"line-header\" data-line=\"tlbr\">TL-BR</th><th colspan=\"5\"></th><th class=\"line-header\" data-line=\"bltr\">BL-TR</th></tr>");
            html.AppendLine("</table>");
            html.AppendLine("<p id=\"status\"></p>");
            html.AppendLine("<p>" + HtmlEncode(GetWinRuleText(card.WinMode)) + "</p>");
            html.AppendLine("<p><em>Standard SRL MM bingo rules apply. Action goals can be completed anytime; item goals must be true at bingo end.</em></p>");
            html.AppendLine("<p><em>Left-click goals to cycle forward; right-click to cycle backward. Left-click row/col headers for vertical popout; right-click for horizontal.</em></p>");
            if (card.RerollTrace != null && card.RerollTrace.Count > 0)
            {
                html.AppendLine("<h2>Reroll Log</h2>");
                html.AppendLine("<pre style=\"white-space:pre-wrap;font-family:Consolas,monospace;font-size:12px;\">");
                html.Append(HtmlEncode(BingoRerollLog.Format(card)));
                html.AppendLine("</pre>");
            }
            html.AppendLine("<div id=\"popout-overlay\"><div id=\"popout-panel\">");
            html.AppendLine("<div id=\"popout-title\"></div><div id=\"popout-grid\"></div></div></div>");
            AppendInteractiveScript(html, card);
            html.AppendLine("</body></html>");
            return html.ToString();
        }

        public static void SavePlainText(BingoCard card, string path)
        {
            File.WriteAllText(path, BuildPlainText(card), Encoding.UTF8);
        }

        public static void SaveHtml(BingoCard card, string path)
        {
            File.WriteAllText(path, BuildHtml(card), Encoding.UTF8);
        }

        private static void AppendMetadata(StringBuilder text, BingoCard card)
        {
            text.Append(BuildMetadataBlock(card));
        }

        private static void AppendMetadataHtml(StringBuilder html, BingoCard card)
        {
            html.AppendLine(HtmlEncode("ROM Seed: " + (card.RomSeed ?? "")) + "<br/>");
            html.AppendLine(HtmlEncode("Generation Seed: " + card.EffectiveSeed) + "<br/>");
            html.AppendLine(HtmlEncode("Pool Hash (" + BingoGoalValidator.PoolHashVersion + "): " + (card.PoolHash ?? "")) + "<br/>");
            html.AppendLine(HtmlEncode("Rerolls: " + card.RerollCount + " | Goals Substituted: " + card.GoalsSubstituted) + "<br/>");
            html.AppendLine(HtmlEncode("Win: " + card.WinMode));
        }

        private static string BuildMetadataBlock(BingoCard card)
        {
            StringBuilder text = new StringBuilder();
            text.AppendLine("ROM Seed: " + (card.RomSeed ?? ""));
            text.AppendLine("Generation Seed: " + card.EffectiveSeed);
            text.AppendLine("Pool Hash (" + BingoGoalValidator.PoolHashVersion + "): " + (card.PoolHash ?? ""));
            text.AppendLine("Rerolls: " + card.RerollCount + " | Goals Substituted: " + card.GoalsSubstituted);
            text.AppendLine("Win: " + card.WinMode);
            return text.ToString().TrimEnd();
        }

        private static void AppendGrid(StringBuilder text, BingoCard card)
        {
            text.AppendLine("        COL-1              COL-2              COL-3              COL-4              COL-5");
            for (int row = 0; row < 5; row++)
            {
                text.Append("ROW-" + (row + 1) + "   ");
                for (int col = 0; col < 5; col++)
                {
                    int index = row * 5 + col;
                    string goal = card.Goals[index];
                    if (goal.Length > 18)
                    {
                        goal = goal.Substring(0, 15) + "...";
                    }

                    text.Append(goal.PadRight(19));
                }

                text.AppendLine();
            }
        }

        private static void AppendRules(StringBuilder text, BingoCard card)
        {
            text.AppendLine(GetWinRuleText(card.WinMode));
            text.AppendLine("Standard SRL MM bingo rules apply.");
        }

        private static string GetWinRuleText(BingoWinMode mode)
        {
            if (mode == BingoWinMode.Blackout)
            {
                return "Win: Blackout — complete all 25 goals.";
            }

            return "Win: Line — complete any row, column, or diagonal (5 goals).";
        }

        private static string FormatGoalHtml(string goal)
        {
            if (string.IsNullOrEmpty(goal))
            {
                return "";
            }

            string normalized = goal.Replace("\r\n", "\n").Replace("\r", "\n");
            string[] parts = normalized.Split('\n');
            StringBuilder html = new StringBuilder();
            for (int i = 0; i < parts.Length; i++)
            {
                if (i > 0)
                {
                    html.Append("<br/>");
                }

                html.Append(HtmlEncode(parts[i]));
            }

            return html.ToString();
        }

        private static void AppendInteractiveScript(StringBuilder html, BingoCard card)
        {
            StringBuilder goalsJson = new StringBuilder("[");
            for (int i = 0; i < 25; i++)
            {
                if (i > 0)
                {
                    goalsJson.Append(",");
                }

                goalsJson.Append("\"").Append(EscapeJsonString(card.Goals[i])).Append("\"");
            }

            goalsJson.Append("]");

            html.AppendLine("<script>");
            html.AppendLine("(function(){");
            html.AppendLine("var goals=" + goalsJson + ";");
            html.AppendLine("var winMode=" + (card.WinMode == BingoWinMode.Blackout ? "\"blackout\"" : "\"line\"") + ";");
            html.AppendLine("var markClasses=[\"\",\"mark-green\",\"mark-red\",\"mark-orange\",\"mark-blue\",\"mark-purple\"];");
            html.AppendLine("function applyState(cell,state){");
            html.AppendLine("cell.setAttribute(\"data-state\",state);");
            html.AppendLine("for(var i=0;i<markClasses.length;i++){if(markClasses[i])cell.classList.remove(markClasses[i]);}");
            html.AppendLine("if(markClasses[state])cell.classList.add(markClasses[state]);}");
            html.AppendLine("function cycle(cell,delta){");
            html.AppendLine("var state=(parseInt(cell.getAttribute(\"data-state\"),10)||0)+delta;");
            html.AppendLine("state=(state+6)%6;applyState(cell,state);updateStatus();}");
            html.AppendLine("function mainCell(index){return document.querySelector('#bingo-grid td.goal-cell[data-index=\"'+index+'\"]');}");
            html.AppendLine("function isMarked(state){return state>0;}");
            html.AppendLine("function lineComplete(indices){");
            html.AppendLine("for(var i=0;i<indices.length;i++){");
            html.AppendLine("var cell=mainCell(indices[i]);");
            html.AppendLine("if(!cell||!isMarked(parseInt(cell.getAttribute(\"data-state\"),10)||0))return false;}");
            html.AppendLine("return true;}");
            html.AppendLine("function updateStatus(){");
            html.AppendLine("var marked=0;");
            html.AppendLine("document.querySelectorAll('#bingo-grid td.goal-cell[data-index]').forEach(function(c){");
            html.AppendLine("if(isMarked(parseInt(c.getAttribute(\"data-state\"),10)||0))marked++;});");
            html.AppendLine("var status=document.getElementById('status');");
            html.AppendLine("if(winMode==='blackout'){status.textContent='Goals: '+marked+' / 25'+(marked>=25?' | Blackout complete!':'');return;}");
            html.AppendLine("var lines=[['ROW-1',[0,1,2,3,4]],['ROW-2',[5,6,7,8,9]],['ROW-3',[10,11,12,13,14]],['ROW-4',[15,16,17,18,19]],['ROW-5',[20,21,22,23,24]],");
            html.AppendLine("['COL-1',[0,5,10,15,20]],['COL-2',[1,6,11,16,21]],['COL-3',[2,7,12,17,22]],['COL-4',[3,8,13,18,23]],['COL-5',[4,9,14,19,24]],");
            html.AppendLine("['TL-BR',[0,6,12,18,24]],['BL-TR',[4,8,12,16,20]]];");
            html.AppendLine("var win='';for(var i=0;i<lines.length;i++){if(lineComplete(lines[i][1])){win=' | Line complete: '+lines[i][0];break;}}");
            html.AppendLine("status.textContent='Goals: '+marked+' / 5 needed'+win;}");
            html.AppendLine("function bindGoalCell(cell){");
            html.AppendLine("cell.addEventListener('mousedown',function(e){");
            html.AppendLine("if(e.button===0){e.preventDefault();cycle(cell,1);}");
            html.AppendLine("else if(e.button===2){e.preventDefault();cycle(cell,-1);}");
            html.AppendLine("});");
            html.AppendLine("cell.addEventListener('contextmenu',function(e){e.preventDefault();});}");
            html.AppendLine("document.querySelectorAll('#bingo-grid td.goal-cell[data-index]').forEach(bindGoalCell);");
            html.AppendLine("var overlay=document.getElementById('popout-overlay');");
            html.AppendLine("var popoutGrid=document.getElementById('popout-grid');");
            html.AppendLine("overlay.addEventListener('mousedown',function(e){if(e.target===overlay)overlay.classList.remove('open');});");
            html.AppendLine("function openPopout(title,indices,horizontal){");
            html.AppendLine("document.getElementById('popout-title').textContent=title;");
            html.AppendLine("popoutGrid.className=horizontal?'horizontal':'vertical';");
            html.AppendLine("popoutGrid.innerHTML='';");
            html.AppendLine("indices.forEach(function(index){");
            html.AppendLine("var main=mainCell(index);");
            html.AppendLine("var cell=document.createElement('div');");
            html.AppendLine("cell.className='popout-cell goal-cell';");
            html.AppendLine("cell.setAttribute('data-index',index);");
            html.AppendLine("cell.innerHTML=main?main.innerHTML:goals[index];");
            html.AppendLine("cell.setAttribute('data-state',main?main.getAttribute('data-state'):'0');");
            html.AppendLine("applyState(cell,parseInt(cell.getAttribute('data-state'),10)||0);");
            html.AppendLine("cell.addEventListener('mousedown',function(e){");
            html.AppendLine("if(e.button===0){e.preventDefault();cycle(cell,1);if(main)applyState(main,parseInt(cell.getAttribute('data-state'),10)||0);updateStatus();}");
            html.AppendLine("else if(e.button===2){e.preventDefault();cycle(cell,-1);if(main)applyState(main,parseInt(cell.getAttribute('data-state'),10)||0);updateStatus();}");
            html.AppendLine("});");
            html.AppendLine("cell.addEventListener('contextmenu',function(e){e.preventDefault();});");
            html.AppendLine("popoutGrid.appendChild(cell);});");
            html.AppendLine("overlay.classList.add('open');}");
            html.AppendLine("function lineIndices(kind,index){");
            html.AppendLine("if(kind==='row'){var s=index*5;return[s,s+1,s+2,s+3,s+4];}");
            html.AppendLine("if(kind==='col'){return[index,index+5,index+10,index+15,index+20];}");
            html.AppendLine("if(kind==='tlbr')return[0,6,12,18,24];");
            html.AppendLine("if(kind==='bltr')return[4,8,12,16,20];");
            html.AppendLine("if(kind==='card'){var a=[];for(var i=0;i<25;i++)a.push(i);return a;}");
            html.AppendLine("return [];}");
            html.AppendLine("document.querySelectorAll('.line-header').forEach(function(h){");
            html.AppendLine("h.addEventListener('mousedown',function(e){");
            html.AppendLine("var kind=h.getAttribute('data-line');");
            html.AppendLine("if(!kind)return;");
            html.AppendLine("var horizontal=e.button===2;");
            html.AppendLine("if(e.button!==0&&e.button!==2)return;");
            html.AppendLine("e.preventDefault();");
            html.AppendLine("var indices=lineIndices(kind,parseInt(h.getAttribute('data-index')||'0',10));");
            html.AppendLine("openPopout(h.textContent,indices,horizontal);");
            html.AppendLine("});");
            html.AppendLine("h.addEventListener('contextmenu',function(e){e.preventDefault();});");
            html.AppendLine("});");
            html.AppendLine("updateStatus();");
            html.AppendLine("})();");
            html.AppendLine("</script>");
        }

        private static string EscapeJsonString(string value)
        {
            if (string.IsNullOrEmpty(value))
            {
                return "";
            }

            return value
                .Replace("\\", "\\\\")
                .Replace("\"", "\\\"")
                .Replace("\r", "\\r")
                .Replace("\n", "\\n");
        }

        private static string HtmlEncode(string value)
        {
            if (string.IsNullOrEmpty(value))
            {
                return "";
            }

            return value
                .Replace("&", "&amp;")
                .Replace("<", "&lt;")
                .Replace(">", "&gt;")
                .Replace("\"", "&quot;");
        }
    }
}
