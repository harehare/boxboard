pub struct BoxId(String);

impl BoxId {
    pub fn new(id: String) -> BoxId {
        BoxId(id)
    }

    pub fn to_string(&self) -> String {
        String::from(&self.0)
    }
}
