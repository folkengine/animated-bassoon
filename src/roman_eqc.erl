-module(roman_eqc).
-include_lib("eqc/include/eqc.hrl").
-compile([export_all]).

positive() ->
	?LET(N, nat(), N + 1).

roman(N) ->
	?LET(R, N, decimal_to_roman(R)). 

prop_roman_add() ->
	?SETUP(fun () -> eqc_c:start(roman),
                     fun() -> ok end
           end,	
	?FORALL({X, Y}, {positive(), positive()},
					begin
						RomanX = decimal_to_roman(X),
						RomanY = decimal_to_roman(Y),
						RomanXY = decimal_to_roman(X + Y),

						equals(
							eqc_c:read_string(roman:add_roman(roman(X), roman(Y))),
							RomanXY)
					end)).


% eqc_gen:sample(eqc_gen:choose(1,5000)).


decimal_to_roman(Num) ->
    decimal_to_roman(Num, "",
        [1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1],
        ["M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX",
         "V", "IV", "I"]).
decimal_to_roman(0, Accum, _, _) -> Accum;
decimal_to_roman(Num, Accum, Decimals, Romans) ->
    F_el = hd(Decimals),
    if
        F_el =< Num -> decimal_to_roman(Num - F_el,
                                        Accum ++ hd(Romans),                    
                                        Decimals, Romans);
        true -> decimal_to_roman(Num, Accum, tl(Decimals), tl(Romans))
    end.

