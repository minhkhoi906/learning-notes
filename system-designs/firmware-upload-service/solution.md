**Functional Requirements:**
- When should the update be triggered? (e.g., periodic, on boot, or manually triggered)
- How should the device verify the update? (version check, hash verification)
- Should the update process be secure? (e.g., signed firmware, encrypted transport)
- How to handle interrupted downloads or update failures? (rollback/fallback mechanism)
- Should the download support partial content / chunking for large firmware files?

**Non-Functional Requirements:**
- Typical firmware size: ~2 GB
- Target update reliability: minimize device downtime and prevent bricking
- Scalability: support thousands/millions of devices simultaneously
- Performance: optimize download speed while avoiding server overload

**Clarifying Questions for the Interview:**
- What is the expected frequency of firmware updates?
- Are updates mandatory or optional?
- Should the device be able to resume interrupted downloads?
- How should we handle version compatibility between firmware and device hardware?
- Should the service provide integrity validation per chunk or per full firmware image?
- What security mechanisms should be enforced? (TLS, digital signatures, authentication)
- Should the download service support range requests for partial content?
- Is there a need for logging/reporting successful or failed updates back to the server?