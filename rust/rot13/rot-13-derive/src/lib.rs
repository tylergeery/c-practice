#![recursion_limit="128"]
extern crate proc_macro;

use crate::proc_macro::TokenStream;
use quote::{quote, quote_spanned};

fn should_rotate(f: &syn::Field) -> bool {
    for attr in &f.attrs {
        let a = attr.interpret_meta();
        if a.is_some() && a.unwrap().name().to_string() == "skip_rot13" {
            return false;
        }
    }

    true
}

fn impl_rot13(ast: &syn::DeriveInput) -> TokenStream {
    let name = &ast.ident;
    match ast.data {
        syn::Data::Struct(ref data_struct) => {
            match data_struct.fields {
                // Iterate over all struct named fields
                syn::Fields::Named(ref fields_named) => {
                    let struct_parts = fields_named.named.iter()
                        .filter(|f| { should_rotate(f) })
                        .map(|f| {
                            let name = &f.ident;
                            quote_spanned! {f.ident.as_ref().unwrap().span()=>
                                #name : String::from_utf8(
                                    self.#name.clone().as_bytes().iter()
                                        .map(|c| {
                                            let val = *c;
                                            if val <= 122 && val >= 110 { return val - 13; }
                                            if val >= 97  && val < 110 { return 122 - (109-val); }
                                            if val <= 90 && val >= 78 { return val - 13; }
                                            if val >= 65  && val < 78 { return 90 - (77-val); }
                                            val
                                        }).collect()
                                    ).unwrap(),
                            }
                        })
                        .chain(
                            fields_named.named.iter()
                                .filter(|f| { !should_rotate(f) })
                                .map(|f| {
                                    let name = &f.ident;
                                    quote_spanned! {f.ident.as_ref().unwrap().span()=>
                                        #name : self.#name.clone(),
                                    }
                                })
                        );

                    let gen = quote! {
                        impl Rot13 for #name {
                            type S = #name;

                            fn rot13(&self) -> Self::S {
                                #name {
                                    #(#struct_parts)*
                                }
                            }
                        }
                    };

                    return gen.into();
                },
                _ => (),
            }
        },
        _ => panic!("Must be a struct"),
    }

    TokenStream::new()
}

#[proc_macro_derive(Rot13)]
pub fn rot13_derive(input: TokenStream) -> TokenStream {
    // Parse the inut
    let ast = syn::parse(input).unwrap();

    impl_rot13(&ast)
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
    }
}
