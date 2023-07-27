/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "tts", "index.html", [
    [ "The Tiny Test System", "index.html", "index" ],
    [ "How-Tos", "usergroup0.html", [
      [ "Setup", "setup.html", null ],
      [ "Getting Started", "tts101.html", null ],
      [ "Command Line Interface", "cli.html", null ],
      [ "Customization Points", "customize.html", null ]
    ] ],
    [ "Reference Documentation", "usergroup1.html", [
      [ "TTS Namespace", "namespacetts.html", null ],
      [ "Macros", "usergroup2.html", [
        [ "Scenario Definition", "usergroup3.html", [
          [ "TTS_CASE", "case_8hpp_a8f02c0809d6d7acc4b7eadd9d269cdc5.html#a8f02c0809d6d7acc4b7eadd9d269cdc5", null ],
          [ "TTS_CASE_TPL", "case_8hpp_a00d91e3b8e2f57672cd95ee48992c707.html#a00d91e3b8e2f57672cd95ee48992c707", null ],
          [ "TTS_CASE_WITH", "case_8hpp_ac910e86136aa7147362154bc6f4544e0.html#ac910e86136aa7147362154bc6f4544e0", null ],
          [ "TTS_WHEN", "when_8hpp_a70b895c13fd5ab455fb7c46226fdc6b3.html#a70b895c13fd5ab455fb7c46226fdc6b3", null ],
          [ "TTS_AND_THEN", "when_8hpp_a73b1883dce7ec54c87702f8f349d52b3.html#a73b1883dce7ec54c87702f8f349d52b3", null ]
        ] ],
        [ "Basic Tests", "usergroup4.html", [
          [ "TTS_EXPECT", "basic_8hpp_ac6b5f64430d5335450f45c6f7b1bbc40.html#ac6b5f64430d5335450f45c6f7b1bbc40", null ],
          [ "TTS_EXPECT_NOT", "basic_8hpp_aaa8c6daa892bf6b6c20d6f049e340567.html#aaa8c6daa892bf6b6c20d6f049e340567", null ],
          [ "TTS_FAIL", "info_8hpp_a251bcaf3210f08e607760669187b79b4.html#a251bcaf3210f08e607760669187b79b4", null ],
          [ "TTS_FATAL", "info_8hpp_ac63862304a02aef33b4a2b7df446c16e.html#ac63862304a02aef33b4a2b7df446c16e", null ],
          [ "TTS_PASS", "info_8hpp_aaba95b4d979dd285e0fb78243a807108.html#aaba95b4d979dd285e0fb78243a807108", null ]
        ] ],
        [ "Relation Tests", "usergroup5.html", [
          [ "TTS_EQUAL", "relation_8hpp_afc88cdb469bdc19411c7b0e9f7f33ae1.html#afc88cdb469bdc19411c7b0e9f7f33ae1", null ],
          [ "TTS_NOT_EQUAL", "relation_8hpp_a8345709fe68ffaec8be13a17cc001257.html#a8345709fe68ffaec8be13a17cc001257", null ],
          [ "TTS_LESS", "relation_8hpp_a7682358fd38e60210da710f7d9ba6399.html#a7682358fd38e60210da710f7d9ba6399", null ],
          [ "TTS_GREATER", "relation_8hpp_afdc655f9fc11b2894b5be4748b3b4cf2.html#afdc655f9fc11b2894b5be4748b3b4cf2", null ],
          [ "TTS_LESS_EQUAL", "relation_8hpp_a1439009af7ca90350dfe8859bde88926.html#a1439009af7ca90350dfe8859bde88926", null ],
          [ "TTS_GREATER_EQUAL", "relation_8hpp_ac4de7d35f38edf73b2a602538100ea23.html#ac4de7d35f38edf73b2a602538100ea23", null ]
        ] ],
        [ "Precision Tests", "usergroup6.html", [
          [ "TTS_ABSOLUTE_EQUAL", "test_2precision_8hpp_af540e2316ab84407390f0b62febfffd1.html#af540e2316ab84407390f0b62febfffd1", null ],
          [ "TTS_IEEE_EQUAL", "test_2precision_8hpp_aab7ac6c9754243b3103e85283cbd2952.html#aab7ac6c9754243b3103e85283cbd2952", null ],
          [ "TTS_RELATIVE_EQUAL", "test_2precision_8hpp_a66fac5cd0984cf9238901308cc60df3e.html#a66fac5cd0984cf9238901308cc60df3e", null ],
          [ "TTS_ULP_EQUAL", "test_2precision_8hpp_a5813c6197c7af5b0ed77d88ce63592a1.html#a5813c6197c7af5b0ed77d88ce63592a1", null ],
          [ "TTS_ULP_RANGE_CHECK", "ranges_8hpp_a7de5e8bb8cb6f1397f2a0a2505def241.html#a7de5e8bb8cb6f1397f2a0a2505def241", null ]
        ] ],
        [ "Sequence Tests", "usergroup7.html", [
          [ "TTS_ALL_EQUAL", "sequence_8hpp_a5a93eb80d6f11fdddb2b00616e8c33b7.html#a5a93eb80d6f11fdddb2b00616e8c33b7", null ],
          [ "TTS_ALL_ABSOLUTE_EQUAL", "sequence_8hpp_ad825e0df73e1fb1669837cdbd8f4ba40.html#ad825e0df73e1fb1669837cdbd8f4ba40", null ],
          [ "TTS_ALL_RELATIVE_EQUAL", "sequence_8hpp_aac398cee8b7655a7e49ed6047d7c233b.html#aac398cee8b7655a7e49ed6047d7c233b", null ],
          [ "TTS_ALL_ULP_EQUAL", "sequence_8hpp_ab7e461674c9066bbd4c333e50595ab09.html#ab7e461674c9066bbd4c333e50595ab09", null ],
          [ "TTS_ALL_IEEE_EQUAL", "sequence_8hpp_aa8195215a7938bde4219b81dbc772a92.html#aa8195215a7938bde4219b81dbc772a92", null ]
        ] ],
        [ "Exceptions Tests", "usergroup8.html", [
          [ "TTS_THROW", "exceptions_8hpp_a55820a3f048b29e7e4f33d0730d072c8.html#a55820a3f048b29e7e4f33d0730d072c8", null ],
          [ "TTS_NO_THROW", "exceptions_8hpp_a73a53c85dca47c0b593e451e30d62bec.html#a73a53c85dca47c0b593e451e30d62bec", null ]
        ] ],
        [ "Constexpr Tests", "usergroup9.html", [
          [ "TTS_CONSTEXPR_EXPECT", "basic_8hpp_a8f27b3b9b08ab6c181cacb2e79a50591.html#a8f27b3b9b08ab6c181cacb2e79a50591", null ],
          [ "TTS_CONSTEXPR_EXPECT_NOT", "basic_8hpp_a09128df27805375d19d923dadc5f2daf.html#a09128df27805375d19d923dadc5f2daf", null ],
          [ "TTS_CONSTEXPR_EQUAL", "relation_8hpp_a8cef7c5be110750a5fccf13db94a913c.html#a8cef7c5be110750a5fccf13db94a913c", null ],
          [ "TTS_CONSTEXPR_NOT_EQUAL", "relation_8hpp_a1c05dc582ca8e663be1f0b1ce98e1d18.html#a1c05dc582ca8e663be1f0b1ce98e1d18", null ],
          [ "TTS_CONSTEXPR_LESS", "relation_8hpp_af7d95b00a74fb828bbd60b50fb4ee18e.html#af7d95b00a74fb828bbd60b50fb4ee18e", null ],
          [ "TTS_CONSTEXPR_GREATER", "relation_8hpp_aebbbbfae70585abf5b6be4428b2c5713.html#aebbbbfae70585abf5b6be4428b2c5713", null ],
          [ "TTS_CONSTEXPR_LESS_EQUAL", "relation_8hpp_a6650da0ab50af14da5642553aa9f0249.html#a6650da0ab50af14da5642553aa9f0249", null ],
          [ "TTS_CONSTEXPR_GREATER_EQUAL", "relation_8hpp_a13898c7492c2bf9ca8b8146cc1510fea.html#a13898c7492c2bf9ca8b8146cc1510fea", null ]
        ] ],
        [ "Type Tests", "usergroup10.html", [
          [ "TTS_TYPE_IS", "test_2types_8hpp_ae409fedd6a2e86ff8cc2f9c832dcff3f.html#ae409fedd6a2e86ff8cc2f9c832dcff3f", null ],
          [ "TTS_EXPR_IS", "test_2types_8hpp_aefac9e4f88dbb637d5457b45fdd7de5f.html#aefac9e4f88dbb637d5457b45fdd7de5f", null ],
          [ "TTS_EXPECT_COMPILES", "test_2types_8hpp_ada89e9e6f4cb5eb147ab4714fa3b9983.html#ada89e9e6f4cb5eb147ab4714fa3b9983", null ],
          [ "TTS_EXPECT_NOT_COMPILES", "test_2types_8hpp_a4047df0595cb9937d9d0406581e66a2f.html#a4047df0595cb9937d9d0406581e66a2f", null ]
        ] ],
        [ "Customization Points", "usergroup11.html", [
          [ "TTS_CUSTOM_DRIVER_FUNCTION", "main_8hpp_ac451ae42ba6d30988c42480129c8bb59.html#ac451ae42ba6d30988c42480129c8bb59", null ],
          [ "TTS_MAIN", "main_8hpp_a38edfacbc423bfee9ab6bcbd24c6eef5.html#a38edfacbc423bfee9ab6bcbd24c6eef5", null ]
        ] ]
      ] ],
      [ "Classes, Aliases and Objects", "usergroup12.html", [
        [ "Data Generators", "usergroup13.html", [
          [ "between", "structtts_1_1between.html", null ],
          [ "ramp", "structtts_1_1ramp.html", null ],
          [ "reverse_ramp", "structtts_1_1reverse__ramp.html", null ],
          [ "sample", "structtts_1_1sample.html", null ],
          [ "value", "structtts_1_1value.html", null ]
        ] ],
        [ "Type Helpers", "usergroup14.html", [
          [ "type", "structtts_1_1type.html", null ],
          [ "types", "structtts_1_1types.html", null ],
          [ "real_types", "namespacetts.html#a533f8005aab86ce53226691c5ae1e7ba", null ],
          [ "int_types", "namespacetts.html#a213ac596b9a0b307be93bdc89f5ba738", null ],
          [ "signed_types", "namespacetts.html#af80f4545b36d2c023830480c853739af", null ],
          [ "uint_types", "namespacetts.html#a19e63a903c9a238a3db10c9d56a9af56", null ],
          [ "integral_types", "namespacetts.html#af94539e8b9b0bc71324e5a5155c97d40", null ],
          [ "arithmetic_types", "namespacetts.html#aa98a8d00dc451d2ba4364c2e6d25d94e", null ],
          [ "typename_", "namespacetts.html#a9baae38fc47577b803dbbb539828e086", null ]
        ] ],
        [ "Runtime Helpers", "usergroup15.html", [
          [ "options", "structtts_1_1options.html", null ],
          [ "prng_generator", "structtts_1_1prng__generator.html", null ],
          [ "realistic_generator", "namespacetts_a6c9aa77a32595083fe37d087e4f77cd3.html#a6c9aa77a32595083fe37d087e4f77cd3", null ],
          [ "realistic_distribution", "namespacetts_acd170d7478bbee9bc3b8a124865fa814.html#acd170d7478bbee9bc3b8a124865fa814", null ]
        ] ]
      ] ],
      [ "Functions", "usergroup16.html", [
        [ "Helpers", "usergroup17.html", [
          [ "arguments", "namespacetts_a9a0ab7d9fa9193a5c1e3b04b729df9b7.html#a9a0ab7d9fa9193a5c1e3b04b729df9b7", null ],
          [ "as_string", "namespacetts_abcc9d7819f7c21822954a11349d9b0e2.html#abcc9d7819f7c21822954a11349d9b0e2", null ],
          [ "name", "namespacetts.html#a35b0fc006475ccee4c7815e669e2689f", null ],
          [ "random_seed", "namespacetts_a15e6f6980d0bc14a1c9fd103c3cd0a63.html#a15e6f6980d0bc14a1c9fd103c3cd0a63", null ]
        ] ],
        [ "Customization Points", "usergroup18.html", [
          [ "as_value", "namespacetts_abda75718fc9250b5bbe3731531eed0ae.html#abda75718fc9250b5bbe3731531eed0ae", null ],
          [ "absolute_distance", "namespacetts_ae29d8013273a5c5c990fea66434ac882.html#ae29d8013273a5c5c990fea66434ac882", null ],
          [ "relative_distance", "namespacetts_a616410f008ed60459668f7b36a2f1dab.html#a616410f008ed60459668f7b36a2f1dab", null ],
          [ "ulp_distance", "namespacetts_a3cd60ebfeba98c825c1e2b796d52be31.html#a3cd60ebfeba98c825c1e2b796d52be31", null ]
        ] ]
      ] ]
    ] ],
    [ "Informations", "usergroup19.html", [
      [ "Rationale", "rationale.html", null ],
      [ "Changelog", "changelog.html", null ],
      [ "Licence", "licence.html", null ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"basic_8hpp_a09128df27805375d19d923dadc5f2daf.html#a09128df27805375d19d923dadc5f2daf"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';