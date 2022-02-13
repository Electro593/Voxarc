/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal terminal
CreateTerminal(heap *Heap,
               font *Font)
{
    terminal Terminal = {0};
    Terminal.LineCount = 0;
    Terminal.MaxLines = TERMINAL__INIT_LINE_COUNT;
    Terminal.Lines = HeapAllocate(Heap, TERMINAL__INIT_LINE_COUNT);
    Terminal.Text = EString(Heap, TERMINAL__INIT_CHAR_COUNT);
    Terminal.Font = Font;
    return Terminal;
}

internal void
WriteToTerminal(terminal *Terminal,
                string String)
{
    if(Terminal->Text.Length + String.Length > Terminal->Text.Capacity) {
        GrowString(Terminal->Text);
    }
    
    
    
    
    
    
}

internal void
DrawTerminal(terminal *Terminal,
             renderer *Renderer)
{
    Assert(Terminal && Terminal->Lines && Terminal->Cursor);
    if(Terminal->LineCount == 0) return;
    
    
    
    
}