import { parse } from "https://deno.land/std/flags/mod.ts";
import { TypescriptParser } from "./typescript.parser.mjs";

//console.log(Deno.args);
    const parsedArgs = parse(Deno.args);
//console.log(parsedArgs);

var json;

if(parsedArgs["_"].length == 0)
{
    const decoder = new TextDecoder("utf-8");
    var code = decoder.decode(await Deno.readAll(Deno.stdin));
    json = await TypescriptParser.parseAsync(code);
}
else
{
    json = await TypescriptParser.parseFileAsync(parsedArgs["_"][0]);
}

//console.log(json);

if("o" in parsedArgs)
{
    const text = new TextEncoder("utf-8").encode(JSON.stringify(json, null, 2)+"\n")
    await Deno.writeFile(parsedArgs["o"], text);
}
else
{
    const text = new TextEncoder("utf-8").encode(JSON.stringify(json, null, 2)+"\n")
    await Deno.writeAll(Deno.stdout, text)
}
