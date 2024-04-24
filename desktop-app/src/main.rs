use log::{warn, info, error};
use egui::{self, Options};
use eframe::{self, App};
mod types;
mod hw_interface;

fn main() -> Result<(), eframe::Error> {
    info!("Starting Application");
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_inner_size([720.0,480.0]),
        ..Default::default()
    };
    eframe::run_native(
    "MRC Flight Computer Programmer",
    options,
    Box::new(|bctx|{
        Box::<Application>::default()
    }))
}
struct Application {
    window_x:f32, 
    window_y:f32,
    current_computer:types::FlightComputerModel,
    avalible_ports:Vec<String>,
    current_port:types::PortSelectorValues,
}
impl Application {
    fn refresh_ports(&mut self) {

    } 
}
impl Default for Application {
    fn default() -> Self {
        Self{
            window_x: 720.0,
            window_y: 480.0,
            current_computer:types::FlightComputerModel::Timer,
            avalible_ports: hw_interface::get_avalible_ports(),
            current_port: types::PortSelectorValues::None,
        }
    }
}
impl eframe::App for Application {
    fn update(&mut self, ctx: &egui::Context, frame: &mut eframe::Frame) {
        egui::CentralPanel::default().show(ctx, |ui|{
            ui.heading("MRC FLIGHT COMPUTER PROGRAMMER");
            if ui.button("Refresh Ports").clicked() {
                self.refresh_ports();

            }
            // processing the ports for the port selector
            let c_port = self.current_port;
            egui::ComboBox::from_label("Select Port...")
            .selected_text(format!("{c_port:?}"))
            .show_ui(ui, |ui|{
                for p in self.avalible_ports {
                    ui.selectable_value(self.current_port, p, p);
                }
                ui.selectable_value(self.current_port, selected_value, "None")
            })
        });
    }
}
