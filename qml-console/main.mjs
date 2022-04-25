import { code2ary } from "./code2ary.mjs";
import { miniMAL } from "./miniMAL.mjs";

console.log("this is main.js!");
//console.log(sum(11, 22));
export function func(glob)
{
    console.log("func() called!");
    var ad = glob.newApplicationData();
    console.log(ad.getTextFromCpp());
    var f = function(){};
    glob.log(typeof f);
    glob.log(typeof function(){});
    glob.log(f);
}
